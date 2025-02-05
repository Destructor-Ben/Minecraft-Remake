#pragma once

#include "Transform.h"

namespace Minecraft
{
    class Texture;
    
    struct Sprite : public Transform
    {
        shared_ptr <Texture> SpriteTexture;
        vec2 UVPosition = vec2(0.0f);
        vec2 UVScale = vec2(1.0f);
        vec3 Color = vec3(1.0f);
    };
}
