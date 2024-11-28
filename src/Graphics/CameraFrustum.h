#pragma once

namespace Minecraft
{
    class BoundingBox;

    enum class CameraPlane
    {
        Left,
        Right,
        Bottom,
        Top,
        Near,
        Far,
        Count,
    };

    // Used for frustum culling
    class CameraFrustum
    {
    public:
        explicit CameraFrustum(mat4 projectionViewMatrix);

        bool ContainsPoint(vec3 point) const;
        bool ContainsBounds(const BoundingBox& bounds);

    private:
        array<vec4, (ulong)CameraPlane::Count> m_Planes;
    };
}
