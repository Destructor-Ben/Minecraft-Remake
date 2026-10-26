#pragma once

#include "Colors.h"
#include "Rectangle.h"
#include "Transform.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    struct Sprite
    {
        vec2i Position = vec2i(0);
        vec2i Origin = vec2i(0); // Translation that occurs before rotation (but not scaling?)
        vec2i Size = vec2i(0);
        float Rotation = 0;
        float Depth = 0.5f; // 0 to 1 range

        optional <Rectangle> UVs = nullopt;
        Color SpriteColor = Colors::White;
        shared_ptr <Texture> SpriteTexture;

        void SetTextureAndScale(shared_ptr <Texture> texture, vec2 scale = vec2(1));
        void SetOriginAsPercent(vec2 percent);
        void SetTargetRect(Rectangle rect); // Shortcut for setting position and size

        mat4 GetTransformationMatrix() const;
    };
}
