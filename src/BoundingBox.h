#pragma once

namespace Minecraft
{
    struct BoundingBox
    {
        vec3 Origin = vec3(0);
        vec3 Size = vec3(1);

        BoundingBox(vec3 origin, vec3 size) : Origin(origin), Size(size) { }

        bool ContainsPoint(vec3 point) const;

        vec3 GetMin() const { return Origin; }
        vec3 GetMax() const { return Origin + Size; }
    };
}
