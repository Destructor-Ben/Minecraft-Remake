#pragma once

#include "Transform.h"

namespace Minecraft
{
    // Handles camera matrices - view and projection
    class Camera : public Transform
    {
    public:
        bool IsPerspective = true;

        float32 FOV = 45.0f;
        float32 OrthographicScale = 1.0f;

        float32 NearClip = 0.1f;
        float32 FarClip = 1000.0f;

        mat4 GetViewMatrix() const;
        mat4 GetProjectionMatrix() const;
    };
}
