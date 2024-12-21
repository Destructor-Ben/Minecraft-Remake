#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class CubeMap;

    class SkyMaterial : public Material
    {
    public:
        shared_ptr <CubeMap> Texture;

        SkyMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
