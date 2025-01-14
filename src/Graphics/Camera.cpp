#include "Camera.h"

#include "Game.h"
#include "Graphics/CameraFrustum.h"

namespace Minecraft
{
    void Camera::Update()
    {
        // Calculate view matrix
        vec3 forward = GetForwardVector();
        ViewMatrix = glm::lookAt(Position, Position + forward, vec3(0, 1, 0));

        // Calculate projection matrix
        if (IsPerspective && Instance->ScreenWidth != 0 && Instance->ScreenHeight != 0)
            ProjectionMatrix = glm::perspective(FOV, (float)Instance->ScreenWidth / (float)Instance->ScreenHeight, NearClip, FarClip);
        else
            ProjectionMatrix = glm::ortho(0.0f, (float)Instance->ScreenWidth * OrthographicScale, 0.0f, (float)Instance->ScreenHeight * OrthographicScale, NearClip, FarClip);

        // Multiply projection and view matrix together
        ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;

        // Calculate frustum
        Frustum = CameraFrustum(ProjectionViewMatrix);
    }
}
