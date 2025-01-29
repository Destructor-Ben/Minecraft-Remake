#pragma once

namespace Minecraft
{
    class Transform
    {
    public:
        vec3 Position = vec3(0.0f);
        quat Rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
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
