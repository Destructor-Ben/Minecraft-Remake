#pragma once

namespace Minecraft
{
    // TODO: make enum class, keep in cpp file
    enum Plane { LEFT = 0, RIGHT, BOTTOM, TOP, NEAR, FAR, PLANE_COUNT };

    // Used for frustum culling
    class CameraFrustum
    {
    public:
        explicit CameraFrustum(mat4 projectionViewMatrix);

        // TODO: bounding boxes too
        bool ContainsPoint(vec3 point) const;

    private:
        array<vec4, PLANE_COUNT> planes;
    };
}
