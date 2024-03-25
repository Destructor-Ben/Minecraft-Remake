#include "Camera.h"

#include "Window.h"

namespace Minecraft
{
    mat4 Camera::GetViewMatrix() const
    {
        vec3 forward = GetForwardVector();
        vec3 up = GetUpVector();
        return glm::lookAt(Position, Position + forward, up);
    }

    // TODO: perspective doesn't remap 0 - Width to -1 to 1 and so forth - is this even an issue? 1 metre looks fine on screen, only really matters for ui rendering
    mat4 Camera::GetProjectionMatrix() const
    {
        if (IsPerspective)
            return glm::perspective(FOV, (float)Window::Width / (float)Window::Height, NearClip, FarClip);
        else
            return glm::ortho(0.0f, (float)Window::Width * OrthographicScale, 0.0f, (float)Window::Height * OrthographicScale, NearClip, FarClip);
    }
}
