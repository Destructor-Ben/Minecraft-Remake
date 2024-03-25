#include "Transform.h"

namespace Minecraft
{
    vec3 Transform::GetForwardVector() const
    {
        vec3 forward;
        forward.x = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
        forward.y = sin(glm::radians(Rotation.x));
        forward.z = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
        return glm::normalize(forward);
    }

    vec3 Transform::GetRightVector() const
    {
        return glm::normalize(glm::cross(GetForwardVector(), GetUpVector()));
    }

    vec3 Transform::GetUpVector() const
    {
        return glm::normalize(vec3(sin(Rotation.z), cos(Rotation.z), 0));
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
        mat4 transform(1.0f);
        transform = glm::translate(transform, Position);
        // TODO: test if this is correct
        transform = glm::rotate(transform, glm::radians(Rotation.x), vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(Rotation.y), vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(Rotation.z), vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, Scale);
        return transform;
    }
}
