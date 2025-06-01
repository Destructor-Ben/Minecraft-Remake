#pragma once

#include "Graphics/Material.h"

namespace Minecraft
{
    class Texture;

    class ChunkMaterial : public Material
    {
    public:
        shared_ptr <Texture> ChunkTexture;
        vec3 AmbientLight = vec3(0);
        vec3 DirToSun = vec3(0);
        vec3 DirToMoon = vec3(0);
        vec3 SunLight = vec3(0);
        vec3 MoonLight = vec3(0);

        ChunkMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
