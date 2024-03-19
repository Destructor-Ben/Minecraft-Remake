#include "Transform.h"

namespace Minecraft
{
    // TODO: handle getting vectors properly - im not sure if all rotation is handled properly
    // TODO: check matrix creation - rotation probably isn't right
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
        return glm::normalize(glm::cross(GetForwardVector(), vec3(0.0f, 1.0f, 0.0f)));
    }

    vec3 Transform::GetUpVector() const
    {
        return glm::normalize(glm::cross(GetRightVector(), GetForwardVector()));
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

    mat4 Transform::GetTransformMatrix() const
    {
        mat4 transform(1.0f);
        transform = glm::translate(transform, Position);
        transform = glm::rotate(transform, Rotation.x, vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, Rotation.y, vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, Rotation.z, vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, Scale);
        return transform;
    }
}
