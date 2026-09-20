#pragma once

#include "Graphics/Material.h"

// TODO: remove the idea of materials in the first place, the state for materials should be in structs, and setting uniforms by the renderer
namespace Minecraft
{
    class Texture;

    class SunMoonMaterial : public Material
    {
    public:
        bool IsSun = true;
        float MinBrightness = 0.35f;
        float SkyDarkness = 0;
        shared_ptr<Texture> SunTexture;
        shared_ptr<Texture> MoonTexture;
        float SunGlowSize = 0;
        float MoonGlowSize = 0;
        float SunGlowStrength = 0;
        float MoonGlowStrength = 0;
        vec3 SunGlowColor;
        vec3 MoonGlowColor;

        SunMoonMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
