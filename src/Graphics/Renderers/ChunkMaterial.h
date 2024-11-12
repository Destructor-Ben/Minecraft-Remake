#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class ChunkMaterial : public Material
    {
    public:
        shared_ptr <Texture> Texture;

        ChunkMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}