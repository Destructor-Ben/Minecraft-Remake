#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class DebugMaterial : public Material
    {
    public:
        vec3 Color = vec3(0);
        bool DrawCircle = false;

        DebugMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
