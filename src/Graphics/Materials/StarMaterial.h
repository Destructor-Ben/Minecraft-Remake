#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class StarMaterial : public Material
    {
    public:
        shared_ptr <Texture> StarTexture;

        StarMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
