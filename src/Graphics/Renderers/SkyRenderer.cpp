#include "SkyRenderer.h"

#include "Game.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/SkyMaterial.h"
#include "Graphics/Materials/SkyObjectMaterial.h"
#include "Graphics/Materials/StarMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "Random/Random.h"
#include "World/World.h"

namespace Minecraft
{
    SkyRenderer::SkyRenderer()
    {
        PrepareSky();
        PrepareStars();
        PrepareSkyObjects();
    }

    void SkyRenderer::Render()
    {
        // Calculate a custom matrix that doesn't include movement
        mat4 projection = Instance->Graphics->SceneCamera->ProjectionMatrix;
        mat4 view = Instance->Graphics->SceneCamera->ViewMatrix;
        view = mat4(mat3(view)); // Remove translation
        mat4 transform = projection * view;

        // Since we draw after the scene, we use a trick to make sure the depth value is always 1
        // This means we need to change the depth function though because otherwise we won't be able to actually write to the pixels
        // But we still need depth testing
        // Also disable depth buffer writing to prevent z-fighting from writing an inaccurate z value
        glDepthFunc(GL_LEQUAL);
        glDepthMask(false);

        // TODO: for every matrix below, it should probably be calculated in the prepare function to avoid wasting performance

        // Calculate the time value for the sky
        // TODO: the "sky gradient time" should be calculated as a brightness value of the day in World, and also used as a brightness value in the chunk renderer
        // TODO: smooth this and make it's change it to be very steep to be more realistic to how light hardly changes much throughout the day
        float skyGradientTime = Instance->CurrentWorld->TimePercent;
        m_SkyMaterial->Time = skyGradientTime;

        // Draw the sky
        // Don't use Renderer.Draw, it is for normal objects
        m_SkyMesh->Draw(transform);

        // Rotate the sky objects while time changes
        // Z axis is east and west
        float skyboxAngle = Instance->CurrentWorld->TimePercent * 2 * numbers::pi;
        transform *= mat4(glm::eulerAngleZ(skyboxAngle));

        // Draw the stars
        // TODO: maybe make stars move at a different speed to the sun + moon?
        // TODO: make night objects fade with brightness
        m_StarMesh->DrawInstanced(transform, m_StarCount);

        // Sun
        constexpr float SunScale = 0.1f;
        m_SkyObjectMaterial->ObjectTexture = m_SunTexture;
        m_SkyObjectMesh->Draw(transform * glm::eulerAngleY((float)-numbers::pi / 2.0f) * glm::translate(vec3(0, 0, -1.0f / SunScale)));

        // Moon
        // TODO: moon phases
        // TODO: maybe make the moon travel slightly faster than the sun, especially if I add moon phases
        constexpr float MoonScale = 0.075f;
        m_SkyObjectMaterial->ObjectTexture = m_MoonTexture;
        m_SkyObjectMesh->Draw(transform * glm::eulerAngleY((float)numbers::pi / 2.0f) * glm::translate(vec3(0, 0, -1.0f / MoonScale)));

        // Reset GL state
        glDepthMask(true);
        glDepthFunc(GL_LESS);
    }

    void SkyRenderer::PrepareSky()
    {
        // Request textures
        auto skyDayGradient = Instance->Graphics->RequestTexture("sky/day-color");
        auto skyNightGradient = Instance->Graphics->RequestTexture("sky/night-color");
        auto sunsetGradient = Instance->Graphics->RequestTexture("sky/sunset-sunrise-color");
        skyDayGradient->SetFilters(GL_LINEAR);
        skyNightGradient->SetFilters(GL_LINEAR);
        sunsetGradient->SetFilters(GL_LINEAR);

        // Create the material
        auto shader = Instance->Graphics->RequestShader("sky");
        m_SkyMaterial = make_shared<SkyMaterial>(shader);
        m_SkyMaterial->DayGradient = skyDayGradient;
        m_SkyMaterial->NightGradient = skyNightGradient;
        m_SkyMaterial->SunsetGradient = sunsetGradient;

        // Create the vertex and index buffers
        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        // Set the mesh data
        vertexBuffer->SetData(
            {
                // Front face (positive Z)
                -1.0f, 1.0f, 1.0f,  // 0: top-left
                -1.0f, -1.0f, 1.0f, // 1: bottom-left
                1.0f, -1.0f, 1.0f,  // 2: bottom-right
                1.0f, 1.0f, 1.0f,   // 3: top-right

                // Back face (negative Z)
                -1.0f, 1.0f, -1.0f,  // 4: top-left
                -1.0f, -1.0f, -1.0f, // 5: bottom-left
                1.0f, -1.0f, -1.0f,  // 6: bottom-right
                1.0f, 1.0f, -1.0f,   // 7: top-right
            }
        );

        indexBuffer->SetData(
            {
                // Front face
                2, 1, 0,
                3, 2, 0,

                // Back face
                4, 5, 6,
                4, 6, 7,

                // Left face
                1, 5, 4,
                0, 1, 4,

                // Right face
                6, 2, 3,
                7, 6, 3,

                // Top face
                3, 0, 4,
                7, 3, 4,

                // Bottom face
                6, 5, 1,
                2, 6, 1,
            }
        );

        // Create the vertex array
        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->AddBuffer(vertexBuffer);

        // Create the mesh
        m_SkyMesh = make_shared<Mesh>(vertexArray);
        m_SkyMesh->AddMaterial(m_SkyMaterial, indexBuffer);
    }

    void SkyRenderer::PrepareStars()
    {
        // Create the material
        auto shader = Instance->Graphics->RequestShader("star");
        m_StarMaterial = make_shared<StarMaterial>(shader);
        m_StarMaterial->StarTexture = Instance->Graphics->RequestTexture("sky/star");

        // Create the stars
        constexpr int StarCount = 500;
        constexpr float StarScale = 0.005f;
        constexpr ulong StarSeed = 0xf63a0f1fc91367d8;

        m_StarCount = StarCount;
        vector <mat4> starMatrices;
        vector<float> starTemperatures;

        // TODO: appropriate random number generator, perhaps wrap Random around NoiseGenerator?
        Random starRandom(StarSeed);

        for (int i = 0; i < StarCount; ++i)
        {
            // Transform
            vec3 position = starRandom.NextPointOnSphere();
            float rotation = starRandom.NextFloat() * numbers::pi * 2;
            float scale = starRandom.NextFloat(0.75f, 1.25f);

            mat4 transform = mat4(1.0f);
            transform *= glm::translate(position); // Position
            transform *= glm::toMat4(glm::quatLookAt(glm::normalize(position), vec3(0, 1, 0))); // Face the world center
            transform *= glm::eulerAngleZ(rotation); // Rotate
            transform *= glm::scale(vec3((StarScale * scale))); // Scale

            starMatrices.push_back(transform);

            // Temperature and brightness
            float temperature = starRandom.NextFloat();
            // TODO: brightness

            starTemperatures.push_back(temperature);
        }

        // Create the mesh
        auto vertexBuffer = CreateQuadVertices();
        auto indexBuffer = CreateQuadIndices();

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->PushFloat(2);
        vertexArray->AddBuffer(vertexBuffer);

        // TODO: abstract all this fancy instancing stuff away
        vertexArray->Bind();

        m_StarMatricesBuffer = make_shared<VertexBuffer>();
        m_StarMatricesBuffer->SetData((const float*)starMatrices.data(), starMatrices.size() * 16); // 16 floats per matrix

        m_StarTemperatureBuffer = make_shared<VertexBuffer>();
        m_StarTemperatureBuffer->SetData(starTemperatures.data(), starTemperatures.size());

        m_StarMatricesBuffer->Bind();

        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(2 + i);
            glVertexAttribPointer(
                2 + i,               // Attribute location
                4,                   // vec4 per column
                GL_FLOAT,            // Data type
                GL_FALSE,            // Normalized
                sizeof(glm::mat4),   // Stride (mat4 is 4 vec4s)
                (void*)(i * sizeof(glm::vec4)) // Offset for each column
            );
            glVertexAttribDivisor(2 + i, 1); // Update once per instance
        }

        m_StarTemperatureBuffer->Bind();
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glVertexAttribDivisor(6, 1);

        m_StarMesh = make_shared<Mesh>(vertexArray);
        m_StarMesh->AddMaterial(m_StarMaterial, indexBuffer);

        VertexArray::Unbind();
    }

    void SkyRenderer::PrepareSkyObjects()
    {
        // Request textures
        m_SunTexture = Instance->Graphics->RequestTexture("sky/sun");
        m_MoonTexture = Instance->Graphics->RequestTexture("sky/moon");

        // Create the material
        auto shader = Instance->Graphics->RequestShader("sky-object");
        m_SkyObjectMaterial = make_shared<SkyObjectMaterial>(shader);

        // Create the vertex and index buffers
        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        // Set the mesh data
        vertexBuffer->SetData(
            {
                -1.0f, 1.0f, -1.0f,
                0.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                0.0f, 0.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 0.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f,
            }
        );

        indexBuffer->SetData(
            {
                0, 1, 2,
                0, 2, 3,
            }
        );

        // Create the vertex arrays
        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->PushFloat(2);
        vertexArray->AddBuffer(vertexBuffer);

        // Create the meshes
        m_SkyObjectMesh = make_shared<Mesh>(vertexArray);
        m_SkyObjectMesh->AddMaterial(m_SkyObjectMaterial, indexBuffer);
    }

    shared_ptr <VertexBuffer> SkyRenderer::CreateQuadVertices()
    {
        auto vertexBuffer = make_shared<VertexBuffer>();

        vertexBuffer->SetData(
            {
                -1.0f, 1.0f, 0.0f,
                0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f,
                0.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f, 0.0f,
                1.0f, 1.0f,
            }
        );

        return vertexBuffer;
    }

    shared_ptr <IndexBuffer> SkyRenderer::CreateQuadIndices()
    {
        auto indexBuffer = make_shared<IndexBuffer>();

        indexBuffer->SetData(
            {
                0, 1, 2,
                0, 2, 3,
            }
        );

        return indexBuffer;
    }
}
