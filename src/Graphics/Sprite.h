#pragma once

#include "Colors.h"
#include "Rectangle.h"
#include "Transform.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    class Texture;

    struct Sprite
    {
        // Ints are pixels, floats are multipliers
        vec2i Position = vec2i(0);
        vec2i Origin = vec2i(0);
        vec2i Size = vec2i(0);
        float Rotation = 0;
        float Depth = 0.5f;
        vec2 Scale = vec2(1);

        optional <Rectangle> UVs = nullopt;
        Color SpriteColor = Colors::White;
        shared_ptr <Texture> SpriteTexture;

        // Sets position and scale and deals with the origin being funky
        void SetTargetRect(Rectangle rect, vec2i origin = vec2i(0));

        mat4 GetTransformationMatrix()
        {
            auto transform = Transform();

            transform.Position = vec3(Position.x, Position.y, -Depth);
            transform.Rotation = glm::rotate(glm::identity<quat>(), Rotation, vec3(0, 0, 1));
            transform.Scale = vec3(SpriteTexture->GetWidth() * Scale.x + Size.x, SpriteTexture->GetHeight() * Scale.y + Size.y, 1);

            return transform.GetTransformationMatrix();
        }
    };
}
