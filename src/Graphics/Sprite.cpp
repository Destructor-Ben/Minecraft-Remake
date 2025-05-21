#include "Sprite.h"

namespace Minecraft
{
    // TODO: fix this
    void Sprite::SetTargetRect(Rectangle rect, vec2i origin)
    {
        Position = rect.GetPosition();
        Origin = origin;
        Scale = vec2(0);
        Size = rect.GetSize();
    }
}
