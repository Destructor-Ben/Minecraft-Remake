#pragma once

namespace Minecraft
{
    // Note that no error checking happens with this because I want min and max to be exposed
    // TODO: verify frustum culling and debug bounds rendering still work
    struct BoundingBox
    {
        vec3 Min = vec3(0);
        vec3 Max = vec3(1);

        BoundingBox(vec3 min, vec3 max) : Min(min), Max(max) { }

        vec3 GetSize() const { return Max - Min; }

        bool ContainsPoint(vec3 point) const;
        BoundingBox Transformed(mat4 transform) const;
    };
}
