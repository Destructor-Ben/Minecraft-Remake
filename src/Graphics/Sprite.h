#pragma once

#include "Rectangle.h"
#include "Transform.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    class Texture;

    // TODO: FromRect function - deals with origin?
    struct Sprite
    {
        // These aren't forced to be ints so be careful
        // TODO: maybe force to be ints
        vec2 Position = vec2(0);
        vec2 Origin = vec2(0);
        float Rotation = 0;
        float Depth = 0.5f;
        vec2 Scale = vec2(1); // Multiplier for the texture size
        vec2 Size = vec2(0); // Measured in pixels

        optional<Rectangle> UVs = nullopt; // Measured in pixels
        // TODO: use a colour struct for this
        vec3 Color = vec3(1);
        float Opacity = 1.0f;
        // end TODO
        shared_ptr<Texture> SpriteTexture;

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
