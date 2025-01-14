#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class StarMaterial : public Material
    {
    public:
        float SkyDarkness = 0;
        float TemperatureStrength = 1;
        float MaxBrightness = 1;
        shared_ptr <Texture> StarTexture;
        shared_ptr <Texture> TemperatureGradient;

        StarMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
