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

    // Perspective projection doesn't remap 0:Width and 0:Height to -1:1 (orthographic does), but this is good because 1 block is 1 metre and fits on screen nicely
    mat4 Camera::GetProjectionMatrix() const
    {
        if (IsPerspective)
            return glm::perspective(FOV, (float)Window::Width / (float)Window::Height, NearClip, FarClip);
        else
            return glm::ortho(0.0f, (float)Window::Width * OrthographicScale, 0.0f, (float)Window::Height * OrthographicScale, NearClip, FarClip);
    }
}
