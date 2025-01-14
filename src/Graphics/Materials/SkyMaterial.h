#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class SkyMaterial : public Material
    {
    public:
        float SkyDarkness = 0;
        float SunsetStrength = 0;
        float SunsetCoverage = 0;
        vec3 SunsetColor;
        vec3 SunsetDirection;
        shared_ptr <Texture> DayGradient;
        shared_ptr <Texture> NightGradient;

        SkyMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
