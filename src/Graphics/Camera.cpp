#include "Camera.h"

#include "Window.h"

namespace Minecraft
{
    // TODO: allow roll rotation to work - need to update up vector calculation and also calculate front vector properly
    mat4 Camera::GetViewMatrix() const
    {
        vec3 front = GetFrontVector();
        vec3 up = GetUpVector();

        return glm::lookAt(Position, Position + front, up);
    }

    // TODO: perspective doesn't remap 0 - Width to -1 to 1
    mat4 Camera::GetProjectionMatrix() const
    {
        if (IsPerspective)
            return glm::perspective(FOV, (float)Window::Width / (float)Window::Height, NearClip, FarClip);
        else
            return glm::ortho(0.0f, (float)Window::Width * OrthographicScale, 0.0f, (float)Window::Height * OrthographicScale, NearClip, FarClip);
    }

    vec3 Camera::GetFrontVector() const
    {
        vec3 front;
        front.x = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
        front.y = sin(glm::radians(Rotation.x));
        front.z = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
        front = glm::normalize(front);
        return front;
    }

    vec3 Camera::GetRightVector() const
    {
        return glm::normalize(glm::cross(GetFrontVector(), vec3(0.0f, 1.0f, 0.0f)));
    }

    vec3 Camera::GetUpVector() const
    {
        return glm::normalize(glm::cross(GetRightVector(), GetFrontVector()));
    }
}
