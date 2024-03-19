#include "Camera.h"

#include "Window.h"

namespace Minecraft
{
    mat4 Camera::GetViewMatrix() const
    {
        vec3 front = GetForwardVector();
        vec3 up = GetUpVector();
        return glm::lookAt(Position, Position + front, up);
    }

    // TODO: perspective doesn't remap 0 - Width to -1 to 1 and so forth
    mat4 Camera::GetProjectionMatrix() const
    {
        if (IsPerspective)
            return glm::perspective(FOV, (float)Window::Width / (float)Window::Height, NearClip, FarClip);
        else
            return glm::ortho(0.0f, (float)Window::Width * OrthographicScale, 0.0f, (float)Window::Height * OrthographicScale, NearClip, FarClip);
    }
}
