#pragma once

#include "Graphics/Material.h"
#include "Graphics/Sprite.h"

namespace Minecraft
{
    class Texture;

    class SpriteMaterial : public Material
    {
    public:
        Sprite* DrawnSprite;

        SpriteMaterial(shared_ptr <Shader> shader) : Material(shader) { }

        void Bind() override;
    };
}
