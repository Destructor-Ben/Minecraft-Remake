#include "BoundingBox.h"

namespace Minecraft
{
    bool BoundingBox::ContainsPoint(glm::vec3 point) const
    {
        auto max = GetMax();

        return point.x >= Origin.x
               && point.y >= Origin.y
               && point.z >= Origin.z
               && point.x <= max.x
               && point.y <= max.y
               && point.z <= max.z;
    }
}
