#include "Camera.h"

#include "Game.h"
#include "Graphics/CameraFrustum.h"

namespace Minecraft
{
    void Camera::Update()
    {
        // Calculate view matrix
        vec3 forward = GetForwardVector();
        vec3 up = GetUpVector();
        ViewMatrix = glm::lookAt(Position, Position + forward, up);

        // Calculate projection matrix
        if (IsPerspective && Instance->ScreenWidth != 0 && Instance->ScreenHeight != 0)
            ProjectionMatrix = glm::perspective(FOV, (float)Instance->ScreenWidth / (float)Instance->ScreenHeight, NearClip, FarClip);
        else
            ProjectionMatrix = glm::ortho(0.0f, (float)Instance->ScreenWidth * OrthographicScale, 0.0f, (float)Instance->ScreenHeight * OrthographicScale, NearClip, FarClip);

        // Calculate frustum
        Frustum = CameraFrustum(ProjectionMatrix * ViewMatrix);
    }
}
