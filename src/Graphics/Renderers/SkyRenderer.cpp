#include "SkyRenderer.h"

#include "Game.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/SkyMaterial.h"
#include "Graphics/Materials/SkyObjectMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "Random/NoiseGenerator.h"
#include "World/World.h"

namespace Minecraft
{
    SkyRenderer::SkyRenderer()
    {
        PrepareSky();
        PrepareSkyObjects();
    }

    void SkyRenderer::Render()
    {
        mat4 projection = Instance->Graphics->SceneCamera->ProjectionMatrix;
        mat4 view = Instance->Graphics->SceneCamera->ViewMatrix;
        view = mat4(mat3(view)); // Remove translation
        mat4 transform = projection * view;

        // Rotate the skybox while time changes
        // Z axis is east and west
        float timeProgress = Instance->CurrentWorld->WorldTime / World::MaxWorldTime;
        float skyboxAngle = numbers::pi * 2 * timeProgress;
        transform *= mat4(glm::eulerAngleZ(skyboxAngle));

        // Since we draw after the scene, we use a trick to make sure the depth value is always 1
        // This means we need to change the depth function though because otherwise we won't be able to actually write to the pixels
        // But we still need depth testing
        // Also disable depth buffer writing to prevent z-fighting from writing an inaccurate z value
        glDepthFunc(GL_LEQUAL);
        glDepthMask(false);

        // Draw the sky
        // Don't use Renderer.Draw, it is for normal objects
        // TODO: for every matrix below, it should probably be calculated in the prepare function to avoid wasting performance
        // TODO: finish making the sky look good - use lookup texture for the colors
        m_SkyMesh->Draw(transform * glm::eulerAngleZ(-(float)numbers::pi / 2));

        // Draw the stars
        // TODO: use instanced rendering for stars
        // TODO: maybe make stars move at a different speed to the sun + moon?
        // TODO: make night objects fade with brightness
        constexpr float StarScale = 0.0025f;
        m_SkyObjectMaterial->ObjectTexture = m_StarTexture;
        for (auto starPos : m_StarPositions)
        {
            m_SkyObjectMesh->Draw(transform * glm::translate(starPos) * mat4(mat3(glm::lookAt(starPos, vec3(0), vec3(0, 1, 0)))) * glm::translate(vec3(0, 0, -1.0f / StarScale)));
        }

        // Sun and moon
        constexpr float SunScale = 0.1f;
        m_SkyObjectMaterial->ObjectTexture = m_SunTexture;
        m_SkyObjectMesh->Draw(transform * glm::eulerAngleY((float)-numbers::pi / 2.0f) * glm::translate(vec3(0, 0, -1.0f / SunScale)));

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
        // Create the material
        auto shader = Instance->Graphics->RequestShader("sky");
        m_SkyMaterial = make_shared<SkyMaterial>(shader);

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

    void SkyRenderer::PrepareSkyObjects()
    {
        // Init star positions
        constexpr int StarCount = 500;
        NoiseGenerator starRandom = { };
        for (int i = 0; i < StarCount; ++i)
        {
            // TODO: proper random
            m_StarPositions.push_back(vec3(starRandom.White1D(i) * 2 - 1, starRandom.White1D(i + 1024) * 2 - 1, starRandom.White1D(i + 2048) * 2 - 1));
        }

        // Request textures
        m_SunTexture = Instance->Graphics->RequestTexture("sky/sun");
        m_MoonTexture = Instance->Graphics->RequestTexture("sky/moon");
        m_StarTexture = Instance->Graphics->RequestTexture("sky/star");

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
}
