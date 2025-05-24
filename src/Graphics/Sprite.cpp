#include "Sprite.h"

namespace Minecraft
{
    void Sprite::SetTextureAndScale(shared_ptr <Texture> texture, vec2 scale)
    {
        SpriteTexture = texture;
        Size = (vec2)texture->GetSize() * scale;
    }

    void Sprite::SetOriginAsPercent(vec2 percent)
    {
        Origin = (vec2)Size * percent;
    }

    void Sprite::SetTargetRect(Rectangle rect)
    {
        Position = rect.GetPosition();
        Size = rect.GetSize();
    }

    mat4 Sprite::GetTransformationMatrix() const
    {
        vec3 position = vec3(Position.x, Position.y, -Depth);
        vec3 origin = vec3(Origin.x, Origin.y, 0);

        // Thank you Grok for helping me understand what I was doing wrong 🙏
        // Do not try to do this in shader because it is way too hard
        mat4 transform = mat4(1.0f);
        transform = glm::translate(transform, position); // Step 3: Translate to final position
        transform = glm::rotate(transform, Rotation, vec3(0, 0, 1)); // Step 2: Rotate around Z-axis
        transform = glm::translate(transform, -origin); // Step 1: Move to origin (before rotation)
        transform = glm::scale(transform, vec3(Size.x, Size.y, 1)); // Step 0: Apply scale

        return transform;
    }
}
