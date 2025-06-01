#pragma once

namespace Minecraft
{
    // Note that no error checking happens with this because I want min and max to be exposed
    struct BoundingBox
    {
        vec3 Min = vec3(0);
        vec3 Max = vec3(1);

        BoundingBox() = default;
        explicit BoundingBox(vec3 min, vec3 max) : Min(min), Max(max) { }

        vec3 GetSize() const { return Max - Min; }

        bool ContainsPoint(vec3 point) const;
        BoundingBox Transformed(mat4 transform) const;

        string to_string() const
        {
            return format("BoundingBox(Min={}, Max={})", glm::to_string(Min), glm::to_string(Max));
        }
    };

    inline std::ostream& operator <<(std::ostream& os, const BoundingBox& bounds)
    {
        os << bounds.to_string();
        return os;
    }

    inline string to_string(const BoundingBox& bounds)
    {
        return bounds.to_string();
    }
}
