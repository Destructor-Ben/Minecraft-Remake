#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Entity
    {
    public:
        vec3 Position;
        vec3 Rotation;
        vec3 Scale;

        mat4 GetModelMatrix() const
        {
            mat4 matrix(1.0f);
            matrix = glm::scale(matrix, Scale);
            matrix = glm::rotate(matrix, glm::length(Rotation), Rotation);
            matrix = glm::translate(matrix, Position);
            return matrix;
        }
    };
}
