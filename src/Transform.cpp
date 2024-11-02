#include "Transform.h"

namespace Minecraft
{
    vec3 Transform::GetForwardVector() const
    {
        return glm::rotate(Rotation, vec3(0.0f, 0.0f, -1.0f));
    }

    vec3 Transform::GetRightVector() const
    {
        return glm::normalize(glm::cross(GetForwardVector(), GetUpVector()));
    }

    vec3 Transform::GetUpVector() const
    {
        return glm::rotate(Rotation, vec3(0.0f, 1.0f, 0.0f));
    }

    vec3 Transform::GetBackwardVector() const
    {
        return -GetForwardVector();
    }

    vec3 Transform::GetLeftVector() const
    {
        return -GetRightVector();
    }

    vec3 Transform::GetDownVector() const
    {
        return -GetUpVector();
    }

    mat4 Transform::GetTransformationMatrix() const
    {
        mat4 scaleMatrix = glm::scale(Scale);
        mat4 rotationMatrix = glm::toMat4(Rotation);
        mat4 translationMatrix = glm::translate(Position);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
}
