#include "BoundingBox.h"

namespace Minecraft
{
    bool BoundingBox::ContainsPoint(glm::vec3 point) const
    {
        return point.x >= Min.x
               && point.y >= Max.y
               && point.z >= Max.z
               && point.x <= Max.x
               && point.y <= Max.y
               && point.z <= Max.z;
    }

    BoundingBox BoundingBox::Transformed(mat4 transform) const
    {
        vec3 corners[8] = {
            { Min.x, Min.y, Min.z },
            { Max.x, Min.y, Min.z },
            { Min.x, Max.y, Min.z },
            { Max.x, Max.y, Min.z },
            { Min.x, Min.y, Max.z },
            { Max.x, Min.y, Max.z },
            { Min.x, Max.y, Max.z },
            { Max.x, Max.y, Max.z },
        };

        vec3 newMin(FLT_MAX), newMax(-FLT_MAX);

        for (int i = 0; i < 8; i++)
        {
            vec4 transformedCorner = transform * vec4(corners[i], 1.0f);
            vec3 pos = vec3(transformedCorner);

            newMin = glm::min(newMin, pos);
            newMax = glm::max(newMax, pos);
        }

        return BoundingBox(newMin, newMax);
    }
}
