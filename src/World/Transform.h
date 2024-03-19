#pragma once

#include "../Common.h"

namespace Minecraft
{
    // Base + instanced class that handles transformations in a 3D world
    class Transform
    {
    public:
        vec3 Position;
        vec3 Rotation;
        vec3 Scale;

        vec3 GetForwardVector() const;
        vec3 GetRightVector() const;
        vec3 GetUpVector() const;
        vec3 GetBackwardVector() const;
        vec3 GetLeftVector() const;
        vec3 GetDownVector() const;

        mat4 GetTransformMatrix() const;
    };
}
