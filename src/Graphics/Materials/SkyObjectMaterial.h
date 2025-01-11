#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class SkyObjectMaterial : public Material
    {
    public:
        shared_ptr <Texture> ObjectTexture;

        SkyObjectMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
