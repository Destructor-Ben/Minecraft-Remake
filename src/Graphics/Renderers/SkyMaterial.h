#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class SkyMaterial : public Material
    {
    public:
        SkyMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
