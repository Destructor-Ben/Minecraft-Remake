#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Camera
    {
    public:
        vec3 Position = vec3(0.0f);
        vec3 Rotation = vec3(0.0f);

        bool IsPerspective = true;

        float FOV = 45.0f;
        float OrthographicScale = 1.0f;

        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        mat4 GetViewMatrix() const;
        mat4 GetProjectionMatrix() const;
    };
}
