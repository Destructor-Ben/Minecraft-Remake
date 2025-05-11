#pragma once

#include "Transform.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    class Texture;

    // TODO: origin for rotation and scaling
    struct Sprite
    {
        vec2 Position = vec2(0);
        float Rotation = 0;
        float Depth = 0.5f;
        float Scale = 1;

        vec2 UVPosition = vec2(0);
        vec2 UVScale = vec2(1);

        vec3 Color = vec3(1);
        shared_ptr<Texture> SpriteTexture;

        mat4 GetTransformationMatrix()
        {
            auto transform = Transform();

            transform.Position = vec3(Position.x, Position.y, -Depth);
            transform.Rotation = glm::rotate(glm::identity<quat>(), Rotation, vec3(0, 0, 1));
            transform.Scale = vec3(SpriteTexture->GetWidth() * Scale, SpriteTexture->GetHeight() * Scale, 1);

            return transform.GetTransformationMatrix();
        }
    };
}
