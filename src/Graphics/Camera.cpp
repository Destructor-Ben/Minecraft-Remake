#include "Camera.h"

namespace Minecraft
{
    void Camera::Update(int screenWidth, int screenHeight)
    {
        vec3 forward = ViewTransform.GetForwardVector();
        ViewMatrix = glm::lookAt(ViewTransform.Position, ViewTransform.Position + forward, vec3(0, 1, 0));

        if (IsPerspective && screenWidth != 0 && screenHeight != 0)
            ProjectionMatrix = glm::perspective(FOV, (float)screenWidth / (float)screenHeight, NearClip, FarClip);
        else
            ProjectionMatrix = glm::ortho(0.0f, (float)screenWidth * OrthographicScale, 0.0f, (float)screenHeight * OrthographicScale, NearClip, FarClip);

        ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;

        Frustum = CameraFrustum(ProjectionViewMatrix);
    }
}
