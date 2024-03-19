#include "Object.h"

namespace Minecraft
{
    // TODO: handle getting vectors
    vec3 Object::GetUpVector() const
    {
        return glm::vec3();
    }

    vec3 Object::GetDownVector() const
    {
        return -GetUpVector();
    }

    vec3 Object::GetLeftVector() const
    {
        return -GetRightVector();
    }

    vec3 Object::GetRightVector() const
    {
        return glm::vec3();
    }

    vec3 Object::GetForwardVector() const
    {
        return glm::vec3();
    }

    vec3 Object::GetBackwardVector() const
    {
        return -GetForwardVector();
    }

    mat4 Object::GetTransform() const
    {
        mat4 transform(1.0f);
        transform = glm::translate(transform, Position);
        transform = glm::rotate(transform, glm::length(Rotation), Rotation);// TODO: is this correct?
        transform = glm::scale(transform, Scale);
        return transform;
    }
}
