#include "BoundingBox.h"

namespace Minecraft
{
    bool BoundingBox::ContainsPoint(glm::vec3 point)
    {
        auto max = Origin + Size;

        return point.x >= Origin.x
               && point.y >= Origin.y
               && point.z >= Origin.z
               && point.x <= max.x
               && point.y <= max.y
               && point.z <= max.z;
    }
}
