#pragma once

#include "../Common.h"

#include "../World/Object.h"

namespace Minecraft
{
    // Handles camera matrices - view and projection
    class Camera : public Object
    {
    public:
        Camera() : Object()
        {
            // Has to have -90 for yaw because cos(0) is 1 or something
            Rotation.y = -90.0f;
        }

        bool IsPerspective = true;

        float FOV = 45.0f;
        float OrthographicScale = 1.0f;

        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        mat4 GetViewMatrix() const;
        mat4 GetProjectionMatrix() const;
    };
}
