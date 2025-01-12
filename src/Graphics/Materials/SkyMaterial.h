#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class SkyMaterial : public Material
    {
    public:
        float Time;
        shared_ptr <Texture> DayGradient;
        shared_ptr <Texture> NightGradient;

        SkyMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
