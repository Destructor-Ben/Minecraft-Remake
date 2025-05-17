#include "Rectangle.h"

namespace Minecraft
{
    bool Rectangle::ContainsPoint(vec2i point)
    {
        return x <= point.x && point.x <= (x + Width) && y <= point.y && point.y <= (y + Height);
    }
}
