#include "UIElement.h"

namespace Minecraft
{
    Rectangle Minecraft::UIElement::GetBounds()
    {
        auto rect = Rectangle();
        rect.x = Position.x - Origin.x;
        rect.y = Position.y - Origin.y;
        rect.Width = Size.x;
        rect.Height = Size.y;
        return rect;
    }
}
