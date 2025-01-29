#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class SunMoonMaterial : public Material
    {
    public:
        bool IsSun = true;
        float MinBrightness = 0.35f;
        float SkyDarkness = 0;
        shared_ptr <Texture> SunTexture;
        shared_ptr <Texture> MoonTexture;

        SunMoonMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
