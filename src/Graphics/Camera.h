#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Camera
    {
    public:
        vec3 Position = vec3(0.0f);
        vec3 Rotation = vec3(0.0f, -90.0f, 0.0f); // Has to have -90 for yaw because cosine

        bool IsPerspective = true;

        float FOV = 45.0f;
        float OrthographicScale = 1.0f;

        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        mat4 GetViewMatrix() const;
        mat4 GetProjectionMatrix() const;

        vec3 GetFrontVector() const;
        vec3 GetRightVector() const;
        vec3 GetUpVector() const;
    };
}
