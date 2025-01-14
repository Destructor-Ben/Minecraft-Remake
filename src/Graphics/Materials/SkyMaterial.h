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
        shared_ptr <Texture> DayGradient;
        shared_ptr <Texture> NightGradient;
        shared_ptr <Texture> SunsetGradient;

        SkyMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
