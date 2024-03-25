#pragma once

#include "../Common.h"

namespace Minecraft
{
    // Base + instanced class that handles transformations in a 3D world
    class Transform
    {
    public:
        vec3 Position = vec3(0.0f);
        // Measured in degrees
        // Defaults -90° for yaw because cos(0) is 1 and cos(90°) is 0 (which we want)
        // TODO: use quaternion for rotation eventually
        vec3 Rotation = vec3(0.0f, -90.0f, 0.0f);
        vec3 Scale = vec3(1.0f);

        vec3 GetForwardVector() const;
        vec3 GetRightVector() const;
        vec3 GetUpVector() const;
        vec3 GetBackwardVector() const;
        vec3 GetLeftVector() const;
        vec3 GetDownVector() const;

        mat4 GetTransformationMatrix() const;
    };
}
