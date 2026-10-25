#pragma once

#include "World/State/PlayerState.h"
#include "World/State/SkyState.h"

namespace Minecraft
{
    class Mesh;
    class Shader;
    class Texture;
}

namespace Minecraft::SkyRenderer
{
    // TODO: delete SunMoonMaterial
    struct State
    {
        mat4 Transform;
        mat4 TransformRotated;
        mat4 SunTransform;
        mat4 MoonTransform;

        shared_ptr<Mesh> SkyMesh;
        shared_ptr<Mesh> StarMesh;
        shared_ptr<Mesh> CelestialObjectMesh;

        shared_ptr<Shader> SkyShader;
        shared_ptr<Shader> StarShader;
        shared_ptr<Shader> CelestialObjectShader;

        shared_ptr<Texture> DayGradient;
        shared_ptr<Texture> NightGradient;
        shared_ptr<Texture> SunTexture;
        shared_ptr<Texture> MoonTexture;
        shared_ptr<Texture> StarTexture;
        shared_ptr<Texture> StarTemperatureGradient;
    };

    void Prepare(State &state, const SkyState &sky);
    void Update(State &state, const SkyState &sky, const PlayerState &player);
    void Render(State &state, const SkyState &sky);
}
