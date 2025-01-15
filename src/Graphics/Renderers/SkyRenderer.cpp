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

    void SkyRenderer::Update()
    {
        float timePercent = Instance->CurrentWorld->TimePercent;

        // Calculate a custom matrix that doesn't include movement
        mat4 projection = Instance->Graphics->SceneCamera->ProjectionMatrix;
        mat4 view = Instance->Graphics->SceneCamera->ViewMatrix;
        view = mat4(mat3(view)); // Remove translation
        m_Transform = projection * view;

        // Rotate the sky objects while time changes
        // Z axis is east and west
        float skyboxAngle = timePercent * 2 * numbers::pi;
        m_TransformRotated = m_Transform * mat4(glm::eulerAngleZ(skyboxAngle));

        // Update the sky values
        UpdateSkyDarkness(timePercent);
        UpdateSunset(timePercent);
    }

    void SkyRenderer::UpdateSkyDarkness(float timePercent)
    {
        // The night gets darker after dusk, and gets light before dawn
        constexpr float FadeTime = 0.05f;

        // While at night
        m_SkyDarkness = 1;

        // Don't show during the day
        if (timePercent <= 0.5f)
            m_SkyDarkness = 0;

        // Common linear equation values
        float gradient = 1 / FadeTime;
        float interceptNightStart = -gradient * 0.5;
        float interceptNightEnd = gradient;

        // After dusk start fading in
        if (0.5f < timePercent && timePercent < 0.5f + FadeTime)
            m_SkyDarkness = gradient * timePercent + interceptNightStart;

        // Before dawn start fading out
        if (1 - FadeTime < timePercent && timePercent < 1)
            m_SkyDarkness = -gradient * timePercent + interceptNightEnd;
    }

    void SkyRenderer::UpdateSunset(float timePercent)
    {
        // Update the sunset direction and coverage
        // We need 90 - Angle because of the working out on paper
        constexpr float Angle = glm::radians(90.0f - 30.0f);
        m_SkyMaterial->SunsetCoverage = 0.25; // TODO: change this line
        // TODO: change the coverage and angle dynamically
        m_SunsetDirection = vec3(-cos(Angle), -sin(Angle), 0);

        // Adjust for east/west with rise/set
        m_SunsetDirection.x *= timePercent <= 0.75f && timePercent >= 0.25 ? 1 : -1;

        // Update the strength
        // Sunsets will fade in linearly, stay for a bit, then fade out
        constexpr float FadeTime = 0.075;
        constexpr float SunsetTime = 0.025f;
        constexpr float HalfSunsetTime = SunsetTime / 2.0f;

        // No sunset
        m_SunsetStrength = 0;

        // Yes sunset
        if (timePercent <= HalfSunsetTime || timePercent >= 1 - HalfSunsetTime || (timePercent >= 0.5f - HalfSunsetTime && timePercent <= 0.5f + HalfSunsetTime))
            m_SunsetStrength = 1;

        // Common linear equation values
        float gradient = 1 / FadeTime;
        float interceptSunsetStart = 1 - gradient * (0.5 - HalfSunsetTime);
        float interceptSunsetEnd = 1 + gradient * (0.5 + HalfSunsetTime);
        float interceptSunriseStart = 1 - gradient * (1 - HalfSunsetTime);
        float interceptSunriseEnd = 1 + gradient * (HalfSunsetTime);

        // Starting sunset
        if (timePercent > 0.5f - HalfSunsetTime - FadeTime && timePercent < 0.5f - HalfSunsetTime)
            m_SunsetStrength = gradient * timePercent + interceptSunsetStart;

        // Ending sunset
        if (timePercent < 0.5f + HalfSunsetTime + FadeTime && timePercent > 0.5f + HalfSunsetTime)
            m_SunsetStrength = -gradient * timePercent + interceptSunsetEnd;

        // Starting sunrise
        if (timePercent > 1.0f - HalfSunsetTime - FadeTime && timePercent < 1.0f - HalfSunsetTime)
            m_SunsetStrength = gradient * timePercent + interceptSunriseStart;

        // Ending sunrise
        if (timePercent < 0.0f + HalfSunsetTime + FadeTime && timePercent > 0.0f + HalfSunsetTime)
            m_SunsetStrength = -gradient * timePercent + interceptSunriseEnd;

        // Make the sunset more visible
        m_SunsetStrength *= 2;
    }

    void SkyRenderer::Render()
    {
        // Since we draw after the scene, we use a trick to make sure the depth value is always 1
        // This means we need to change the depth function though because otherwise we won't be able to actually write to the pixels
        // But we still need depth testing
        // Also disable depth buffer writing to prevent z-fighting from writing an inaccurate z value
        glDepthFunc(GL_LEQUAL);
        glDepthMask(false);

        // Draw the sky
        // Don't use Renderer.Draw, it is for normal objects
        m_SkyMaterial->SkyDarkness = m_SkyDarkness;
        m_SkyMaterial->SunsetStrength = m_SunsetStrength;
        m_SkyMaterial->SunsetDirection = m_SunsetDirection;
        m_SkyMesh->Draw(m_Transform);

        // Draw the stars
        if (m_SkyDarkness != 0)
        {
            m_StarMaterial->SkyDarkness = m_SkyDarkness;
            m_StarMesh->DrawInstanced(m_TransformRotated, m_StarCount);
        }

        // TODO: redo sun and moon rendering
        // TODO: add glow to the sun and moon
        // Sun
        // TODO: for every matrix below, it should probably be calculated in the prepare function to avoid wasting performance
        constexpr float SunScale = 0.1f;
        m_SkyObjectMaterial->ObjectTexture = m_SunTexture;
        m_SkyObjectMesh->Draw(m_TransformRotated * glm::eulerAngleY((float)-numbers::pi / 2.0f) * glm::translate(vec3(0, 0, -1.0f / SunScale)));

        // Moon
        // TODO: moon phases
        // TODO: maybe make the moon travel slightly faster than the sun, especially if I add moon phases
        // TODO: make moon fade with brightness
        // TODO: dont render the moon when its alpha would be zero anyway
        constexpr float MoonScale = 0.075f;
        m_SkyObjectMaterial->ObjectTexture = m_MoonTexture;
        m_SkyObjectMesh->Draw(m_TransformRotated * glm::eulerAngleY((float)numbers::pi / 2.0f) * glm::translate(vec3(0, 0, -1.0f / MoonScale)));

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
        auto shader = Instance->Graphics->RequestShader("sky/sky");
        m_SkyMaterial = make_shared<SkyMaterial>(shader);
        m_SkyMaterial->SunsetColor = vec3(251.0f / 255.0f, 130.0f / 255.0f, 9.0f / 255.0f);
        m_SkyMaterial->DayGradient = skyDayGradient;
        m_SkyMaterial->NightGradient = skyNightGradient;

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
        auto shader = Instance->Graphics->RequestShader("sky/star");
        m_StarMaterial = make_shared<StarMaterial>(shader);
        m_StarMaterial->TemperatureStrength = 0.3;
        m_StarMaterial->MaxBrightness = 0.9;
        m_StarMaterial->StarTexture = Instance->Graphics->RequestTexture("sky/star");
        m_StarMaterial->TemperatureGradient = Instance->Graphics->RequestTexture("sky/star-temperature");

        // Create the stars
        constexpr int StarCount = 750;
        constexpr float StarScale = 0.005f;
        constexpr ulong StarSeed = 0xf63a0f1fc91367d8;

        m_StarCount = StarCount;
        vector <mat4> starMatrix;
        vector<float> starBrightness;
        vector<float> starTemperature;

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

            starMatrix.push_back(transform);

            // Brightness and temperature
            float brightness = starRandom.NextFloat();
            float temperature = starRandom.NextFloat();

            starBrightness.push_back(brightness);
            starTemperature.push_back(temperature);
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

        m_StarMatrixBuffer = make_shared<VertexBuffer>();
        m_StarMatrixBuffer->SetData((const float*)starMatrix.data(), starMatrix.size() * 16); // 16 floats per matrix

        m_StarBrightnessBuffer = make_shared<VertexBuffer>();
        m_StarBrightnessBuffer->SetData(starBrightness.data(), starBrightness.size());

        m_StarTemperatureBuffer = make_shared<VertexBuffer>();
        m_StarTemperatureBuffer->SetData(starTemperature.data(), starTemperature.size());

        m_StarMatrixBuffer->Bind();

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

        m_StarBrightnessBuffer->Bind();
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glVertexAttribDivisor(6, 1);

        m_StarTemperatureBuffer->Bind();
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glVertexAttribDivisor(7, 1);

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
        auto shader = Instance->Graphics->RequestShader("sky/sky-object");
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
