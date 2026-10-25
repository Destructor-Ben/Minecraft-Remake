#include "SkyRenderer.h"

#include "Game.h"
#include "Graphics/GL.h"
#include "ResourceManager.h"
#include "Random/Random.h"
#include "World/State/PlayerState.h"
#include "World/State/SkyState.h"
#include <algorithm>

namespace Minecraft::SkyRenderer
{
    static void PrepareSky(State &state, const SkyState &sky);
    static void PrepareStars(State &state, const SkyState &sky);
    static void PrepareSunAndMoon(State &state, const SkyState &sky);
    static void RenderCelestialObject(State &state, shared_ptr<Texture> texture, mat4 transform, float brightness, const CelestialObject &obj);

    static shared_ptr<VertexBuffer> CreateQuadVertices();
    static shared_ptr<IndexBuffer> CreateQuadIndices();

    void Prepare(State &state, const SkyState &sky)
    {
        PrepareSky(state, sky);
        PrepareStars(state, sky);
        PrepareSunAndMoon(state, sky);
    }

    void Update(State &state, const SkyState &sky, const PlayerState &player)
    {
        mat4 projection = player.PlayerCamera.ProjectionMatrix;
        mat4 view = player.PlayerCamera.ViewMatrix;
        view = mat4(mat3(view)); // Remove translation
        state.Transform = projection * view;
        state.TransformRotated = state.Transform * sky.CelestialObjectRotation;
    }

    void Render(State &state, const SkyState &sky)
    {
        // Since we draw after the scene, we use a trick to make sure the depth value is always 1
        // This means we need to change the depth function though because otherwise we won't be able to actually write to the pixels
        // But we still need depth testing
        // Also disable depth buffer writing to prevent z-fighting from writing an inaccurate z value
        glDepthFunc(GL_LEQUAL);
        glDepthMask(false);

        state.SkyShader->Bind(); // TODO: avoid doing this twice! perhaps bind the textures permanently in Prepare
        // - allow Draw to take in a function to bind the params? or just make Mesh::Draw less stupid. Multiple materials is not a use case for this game, just use multiple meshes referencing the same mesh data
        state.SkyShader->SetUniform("uDayGradient", state.DayGradient, 0);
        state.SkyShader->SetUniform("uNightGradient", state.NightGradient, 1);
        state.SkyShader->SetUniform("uSunsetColor", sky.Config.Sunset.MainColor.RGB);
        state.SkyShader->SetUniform("uSkyDarkness", sky.Darkness);
        state.SkyShader->SetUniform("uSunsetStrength", sky.SunsetStrength);
        state.SkyShader->SetUniform("uSunsetCoverage", sky.Config.Sunset.Coverage);
        state.SkyShader->SetUniform("uSunsetDirection", sky.SunsetDirection);
        state.SkyMesh->Draw(state.Transform); // Don't use Renderer.Draw, it is for normal objects

        if (sky.Darkness != 0)
        {
            state.StarShader->Bind(); // TODO: avoid doing this twice
            state.StarShader->SetUniform("uTexture", state.StarTexture, 0);
            state.StarShader->SetUniform("uTemperatureGradient", state.StarTemperatureGradient, 1);
            state.StarShader->SetUniform("uSkyDarkness", sky.Darkness);
            state.StarShader->SetUniform("uTemperatureStrength", sky.Config.Stars.TemperatureStrength);
            state.StarShader->SetUniform("uTwinkleStrength", sky.Config.Stars.TwinkleStrength);
            state.StarShader->SetUniform("uMaxBrightness", sky.Config.Stars.MaxBrightness);
            state.StarShader->SetUniform("uTime", Instance->ElapsedSeconds);
            state.StarMesh->DrawInstanced(state.TransformRotated, sky.Config.Stars.Count);
        }

        RenderCelestialObject(state, state.SunTexture, state.SunTransform, 1.0f, sky.Config.Sun);
        RenderCelestialObject(state, state.MoonTexture, state.MoonTransform, std::max(sky.Darkness, 0.35f), sky.Config.Moon);

        // Reset GL state
        glDepthMask(true);
        glDepthFunc(GL_LESS);
    }

    void RenderCelestialObject(State &state, shared_ptr<Texture> texture, mat4 transform, float brightness, const CelestialObject &obj)
    {
        state.CelestialObjectShader->Bind();
        state.CelestialObjectShader->SetUniform("uBrightness", brightness);
        state.CelestialObjectShader->SetUniform("uTexture", texture, 0);
        state.CelestialObjectShader->SetUniform("uGlowSize", obj.GlowSize);
        state.CelestialObjectShader->SetUniform("uGlowStrength", obj.GlowStrength);
        state.CelestialObjectShader->SetUniform("uGlowColor", obj.GlowColor.RGB);
        state.CelestialObjectMesh->Draw(state.TransformRotated * transform);
    }

    shared_ptr<VertexBuffer> CreateQuadVertices()
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

    shared_ptr<IndexBuffer> CreateQuadIndices()
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

    void PrepareSky(State &state, const SkyState &sky)
    {
        state.SkyShader = Resources::RequestShader("sky/sky");
        state.DayGradient = Resources::RequestTexture("sky/day-color");
        state.NightGradient = Resources::RequestTexture("sky/night-color");
        state.DayGradient->SetFilters(GL_LINEAR);
        state.NightGradient->SetFilters(GL_LINEAR);

        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        vertexBuffer->SetData(
            {
                // Front face
                -1.0f, 1.0f, 1.0f,  // Top left
                -1.0f, -1.0f, 1.0f, // Bottom left
                1.0f, -1.0f, 1.0f,  // Bottom right
                1.0f, 1.0f, 1.0f, // Top right
                // Back face
                -1.0f, 1.0f, -1.0f,  // Top left
                -1.0f, -1.0f, -1.0f, // Bottom left
                1.0f, -1.0f, -1.0f,  // Bottom right
                1.0f, 1.0f, -1.0f,   // Top right
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

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->AddBuffer(vertexBuffer);

        auto material = make_shared<Material>(state.SkyShader);
        state.SkyMesh = make_shared<Mesh>(vertexArray);
        state.SkyMesh->Materials[material] = indexBuffer;
    }

    void PrepareStars(State &state, const SkyState &sky)
    {
        state.StarShader = Resources::RequestShader("sky/star");
        state.StarTexture = Resources::RequestTexture("sky/star");
        state.StarTemperatureGradient = Resources::RequestTexture("sky/star-temperature");
        state.StarTemperatureGradient->SetFilters(GL_LINEAR);

        // Create the stars
        vector<mat4> starMatrix;
        vector<float> starBrightness;
        vector<float> starTemperature;
        vector<float> starTwinkleSpeed;
        vector<float> starTwinkleOffset;
        vector<int> starTextureIndex;

        Random starRandom(sky.Config.Stars.Seed);

        for (int i = 0; i < sky.Config.Stars.Count; ++i)
        {
            vec3 position = starRandom.NextPointOnSphere();
            float rotation = starRandom.NextFloat() * numbers::pi * 2;
            float scalePercentVariance = sky.Config.Stars.ScalePercentVariance;
            float scale = starRandom.NextFloat(1.0 - scalePercentVariance, 1.0 + scalePercentVariance);

            mat4 transform = mat4(1.0f);
            transform *= glm::translate(position);
            transform *= glm::toMat4(glm::quatLookAt(glm::normalize(position), vec3(0, 1, 0)));
            transform *= glm::eulerAngleZ(rotation);
            transform *= glm::scale(vec3((sky.Config.Stars.Scale * scale)));

            starMatrix.push_back(transform);

            float brightness = starRandom.NextFloat();
            float temperature = starRandom.NextFloat();
            float twinkleSpeed = starRandom.NextFloat(sky.Config.Stars.MinTwinkleSpeed, sky.Config.Stars.MaxTwinkleSpeed);
            float twinkleOffset = starRandom.NextFloat();
            int textureIndex = starRandom.NextInt(0, 2);

            starBrightness.push_back(brightness);
            starTemperature.push_back(temperature);
            starTwinkleSpeed.push_back(twinkleSpeed);
            starTwinkleOffset.push_back(twinkleOffset);
            starTextureIndex.push_back(textureIndex);
        }

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

        auto material = make_shared<Material>(state.StarShader);
        state.StarMesh = make_shared<Mesh>(vertexArray);
        state.StarMesh->Materials[material] = indexBuffer;

        VertexArray::Unbind();
    }

    void PrepareSunAndMoon(State &state, const SkyState &sky)
    {
        state.CelestialObjectShader = Resources::RequestShader("sky/celestial-object");
        state.SunTexture = Resources::RequestTexture("sky/sun");
        state.MoonTexture = Resources::RequestTexture("sky/moon");

        auto vertexBuffer = CreateQuadVertices();
        auto indexBuffer = CreateQuadIndices();

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->PushFloat(2);
        vertexArray->AddBuffer(vertexBuffer);

        auto material = make_shared<Material>(state.CelestialObjectShader);
        state.CelestialObjectMesh = make_shared<Mesh>(vertexArray);
        state.CelestialObjectMesh->Materials[material] = indexBuffer;

        auto sunTransform = Transform();
        sunTransform.Position.x = 1;
        sunTransform.Rotation = quat(glm::eulerAngleY(-numbers::pi / 2.0f));
        sunTransform.Scale *= sky.Config.Sun.Scale;

        auto moonTransform = Transform();
        moonTransform.Position.x = -1;
        moonTransform.Rotation = quat(glm::eulerAngleY(numbers::pi / 2.0f));
        moonTransform.Rotation *= quat(glm::eulerAngleZ(numbers::pi));
        moonTransform.Scale *= sky.Config.Moon.Scale;

        state.SunTransform = sunTransform.GetTransformationMatrix();
        state.MoonTransform = moonTransform.GetTransformationMatrix();
    }
}
