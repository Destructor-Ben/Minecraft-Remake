#pragma once

#include "Transform.h"

// TODO: make a camera frustum class, bounds class, and make the renderer use frustum culling
namespace Minecraft
{
    class CameraFrustum;

    // Handles camera matrices - view and projection
    class Camera : public Transform
    {
    public:
        bool IsPerspective = true;

        float FOV = 45.0f;
        float OrthographicScale = 1.0f;

        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        mat4 GetViewMatrix() const;
        mat4 GetProjectionMatrix() const;

        CameraFrustum GetFrustum() const;
    };
}
