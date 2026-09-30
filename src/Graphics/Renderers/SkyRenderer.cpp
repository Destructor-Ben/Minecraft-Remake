#include "SkyRenderer.h"

#include "Color.h"
#include "Colors.h"
#include "Game.h"
#include "Logger.h"
#include "Profiler.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/SkyMaterial.h"
#include "Graphics/Materials/SunMoonMaterial.h"
#include "Graphics/Materials/StarMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "Random/Random.h"
#include "World/World.h"

#include <yaml-cpp/yaml.h>

namespace Minecraft
{
    SkyRenderer::SkyRenderer()
    {
        // TODO: make a FetchConfig function that fetches config values and stores them in state structs
        // TODO: move all material fields to state variables and remove materials
        m_Config = Resources::RequestConfig("graphics/sky");

        PrepareSky();
        PrepareStars();
        PrepareSunAndMoon();
    }

    void SkyRenderer::PrepareSky()
    {
        // TODO: store this in a struct, perhaps sky visual state
        auto sunsetColor = m_Config.TryGetValue<Color>("sunset-color").value_or(Colors::White);

        auto skyDayGradient = Resources::RequestTexture("sky/day-color");
        auto skyNightGradient = Resources::RequestTexture("sky/night-color");
        skyDayGradient->SetFilters(GL_LINEAR);
        skyNightGradient->SetFilters(GL_LINEAR);

        // Create the material
        auto shader = Resources::RequestShader("sky/sky");
        m_SkyMaterial = make_shared<SkyMaterial>(shader);
        m_SkyMaterial->SunsetColor = sunsetColor.RGB;
        m_SkyMaterial->DayGradient = skyDayGradient;
        m_SkyMaterial->NightGradient = skyNightGradient;

        // Create the vertex and index buffers
        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        // TODO: just use my quad system to create the mesh data

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
        m_SkyMesh->Materials[m_SkyMaterial] = indexBuffer;
    }

    void SkyRenderer::PrepareStars()
    {
        // Create the material
        auto shader = Resources::RequestShader("sky/star");
        m_StarMaterial = make_shared<StarMaterial>(shader);
        m_StarMaterial->TemperatureStrength = 0.35f;
        m_StarMaterial->TwinkleStrength = 0.75f;
        m_StarMaterial->MaxBrightness = 0.9f;
        m_StarMaterial->StarTexture = Resources::RequestTexture("sky/star");
        m_StarMaterial->TemperatureGradient = Resources::RequestTexture("sky/star-temperature");
        m_StarMaterial->TemperatureGradient->SetFilters(GL_LINEAR);

        // Create the stars
        m_StarCount = m_Config.TryGetValue<int>("stars/count").value_or(100);
        ulong starSeed = m_Config.TryGetValue<ulong>("stars/seed").value_or(123);
        float starScale = m_Config.TryGetValue<float>("stars/scale").value_or(1.0f);
        float starScalePercentVariance = m_Config.TryGetValue<float>("stars/scale-percent-variance").value_or(0.0f);
        float minTwinkleSpeed = m_Config.TryGetValue<float>("stars/min-twinkle-speed").value_or(0.0f);
        float maxTwinkleSpeed = m_Config.TryGetValue<float>("stars/max-twinkle-speed").value_or(0.0f);

        vector<mat4> starMatrix;
        vector<float> starBrightness;
        vector<float> starTemperature;
        vector<float> starTwinkleSpeed;
        vector<float> starTwinkleOffset;
        vector<int> starTextureIndex;

        Random starRandom(starSeed);

        for (int i = 0; i < m_StarCount; ++i)
        {
            vec3 position = starRandom.NextPointOnSphere();
            float rotation = starRandom.NextFloat() * numbers::pi * 2;
            float scale = starRandom.NextFloat(1.0 - starScalePercentVariance, 1.0 + starScalePercentVariance);

            mat4 transform = mat4(1.0f);
            transform *= glm::translate(position);
            transform *= glm::toMat4(glm::quatLookAt(glm::normalize(position), vec3(0, 1, 0)));
            transform *= glm::eulerAngleZ(rotation);
            transform *= glm::scale(vec3((starScale * scale)));

            starMatrix.push_back(transform);

            float brightness = starRandom.NextFloat();
            float temperature = starRandom.NextFloat();
            float twinkleSpeed = starRandom.NextFloat(minTwinkleSpeed, maxTwinkleSpeed);
            float twinkleOffset = starRandom.NextFloat();
            int textureIndex = starRandom.NextInt(0, 2);

            starBrightness.push_back(brightness);
            starTemperature.push_back(temperature);
            starTwinkleSpeed.push_back(twinkleSpeed);
            starTwinkleOffset.push_back(twinkleOffset);
            starTextureIndex.push_back(textureIndex);
        }

        // Create the mesh
        auto vertexBuffer = CreateQuadVertices();
        auto indexBuffer = CreateQuadIndices();

        auto starMatrixBuffer = make_shared<VertexBuffer>();
        auto starBrightnessBuffer = make_shared<VertexBuffer>();
        auto starTemperatureBuffer = make_shared<VertexBuffer>();
        auto starTwinkleSpeedBuffer = make_shared<VertexBuffer>();
        auto starTwinkleOffsetBuffer = make_shared<VertexBuffer>();
        auto starTextureIndexBuffer = make_shared<VertexBuffer>();

        starMatrixBuffer->SetData((const float*)starMatrix.data(), starMatrix.size() * 16); // 16 floats per matrix
        starBrightnessBuffer->SetData(starBrightness);
        starTemperatureBuffer->SetData(starTemperature);
        starTwinkleSpeedBuffer->SetData(starTwinkleSpeed);
        starTwinkleOffsetBuffer->SetData(starTwinkleOffset);
        starTextureIndexBuffer->SetDataRaw(starTextureIndex.data(), starTextureIndex.size() * sizeof(int));

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->PushFloat(2);
        vertexArray->AddBuffer(vertexBuffer);
        vertexArray->PushMat4(true);
        vertexArray->AddBuffer(starMatrixBuffer);
        vertexArray->PushFloat(1, true);
        vertexArray->AddBuffer(starBrightnessBuffer);
        vertexArray->PushFloat(1, true);
        vertexArray->AddBuffer(starTemperatureBuffer);
        vertexArray->PushFloat(1, true);
        vertexArray->AddBuffer(starTwinkleSpeedBuffer);
        vertexArray->PushFloat(1, true);
        vertexArray->AddBuffer(starTwinkleOffsetBuffer);
        vertexArray->PushInt(1, true);
        vertexArray->AddBuffer(starTextureIndexBuffer);

        m_StarMesh = make_shared<Mesh>(vertexArray);
        m_StarMesh->Materials[m_StarMaterial] = indexBuffer;

        VertexArray::Unbind();
    }

    void SkyRenderer::PrepareSunAndMoon()
    {
        // Create the material
        auto sunTexture = Resources::RequestTexture("sky/sun");
        auto moonTexture = Resources::RequestTexture("sky/moon");

        auto shader = Resources::RequestShader("sky/sun-moon");
        m_SunAndMoonMaterial = make_shared<SunMoonMaterial>(shader);
        m_SunAndMoonMaterial->SunTexture = sunTexture;
        m_SunAndMoonMaterial->MoonTexture = moonTexture;
        m_SunAndMoonMaterial->SunGlowColor = m_Config.TryGetValue<Color>("sun-glow/color").value_or(Colors::Magenta).RGB;
        m_SunAndMoonMaterial->MoonGlowColor = m_Config.TryGetValue<Color>("moon-glow/color").value_or(Colors::Magenta).RGB;
        m_SunAndMoonMaterial->SunGlowSize = m_Config.TryGetValue<float>("sun-glow/size").value_or(1.0);
        m_SunAndMoonMaterial->MoonGlowSize = m_Config.TryGetValue<float>("moon-glow/size").value_or(1.0);
        m_SunAndMoonMaterial->SunGlowStrength = m_Config.TryGetValue<float>("sun-glow/strength").value_or(1.0);
        m_SunAndMoonMaterial->MoonGlowStrength = m_Config.TryGetValue<float>("moon-glow/strength").value_or(1.0);

        // Create the mesh
        auto vertexBuffer = CreateQuadVertices();
        auto indexBuffer = CreateQuadIndices();

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->PushFloat(2);
        vertexArray->AddBuffer(vertexBuffer);

        m_SunAndMoonMesh = make_shared<Mesh>(vertexArray);
        m_SunAndMoonMesh->Materials[m_SunAndMoonMaterial] = indexBuffer;

        // Set transforms
        auto sunTransform = Transform();
        sunTransform.Position.x = 1;
        sunTransform.Rotation = quat(glm::eulerAngleY(-numbers::pi / 2.0f));
        sunTransform.Scale *= 0.25f;

        auto moonTransform = Transform();
        moonTransform.Position.x = -1;
        moonTransform.Rotation = quat(glm::eulerAngleY(numbers::pi / 2.0f));
        moonTransform.Rotation *= quat(glm::eulerAngleZ(numbers::pi));
        moonTransform.Scale *= 0.1f;

        m_SunTransform = sunTransform.GetTransformationMatrix();
        m_MoonTransform = moonTransform.GetTransformationMatrix();
    }

    void SkyRenderer::Update(SkyState &sky)
    {
        mat4 projection = Instance->CurrentWorld->Player.PlayerCamera.ProjectionMatrix;
        mat4 view = Instance->CurrentWorld->Player.PlayerCamera.ViewMatrix;
        view = mat4(mat3(view)); // Remove translation
        m_Transform = projection * view;
        m_TransformRotated = m_Transform * sky.CelestialObjectRotation;

        m_SunAndMoonMaterial->SkyDarkness = sky.Darkness;

        m_SkyMaterial->SkyDarkness = sky.Darkness;
        m_SkyMaterial->SunsetStrength = sky.SunsetStrength;
        m_SkyMaterial->SunsetCoverage = sky.SunsetCoverage;
        m_SkyMaterial->SunsetDirection = sky.SunsetDirection;

        m_StarMaterial->SkyDarkness = sky.Darkness;
        m_StarMaterial->Time = Instance->ElapsedSeconds;
    }

    void SkyRenderer::Render()
    {
        Instance->PerfProfiler->Push("SkyRenderer::Render");

        // Since we draw after the scene, we use a trick to make sure the depth value is always 1
        // This means we need to change the depth function though because otherwise we won't be able to actually write to the pixels
        // But we still need depth testing
        // Also disable depth buffer writing to prevent z-fighting from writing an inaccurate z value
        glDepthFunc(GL_LEQUAL);
        glDepthMask(false);

        // Draw the sky
        // Don't use Renderer.Draw, it is for normal objects
        m_SkyMesh->Draw(m_Transform);

        // Draw the stars
        if (m_StarMaterial->SkyDarkness != 0)
            m_StarMesh->DrawInstanced(m_TransformRotated, m_StarCount);

        // Sun and moon
        m_SunAndMoonMaterial->IsSun = true;
        m_SunAndMoonMesh->Draw(m_TransformRotated * m_SunTransform);

        m_SunAndMoonMaterial->IsSun = false;
        m_SunAndMoonMesh->Draw(m_TransformRotated * m_MoonTransform);

        // Reset GL state
        glDepthMask(true);
        glDepthFunc(GL_LESS);

        Instance->PerfProfiler->Pop();
    }


    // TODO: just use my quad system to create the mesh data

    shared_ptr<VertexBuffer> SkyRenderer::CreateQuadVertices()
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

    shared_ptr<IndexBuffer> SkyRenderer::CreateQuadIndices()
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
