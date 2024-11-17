#include "CameraFrustum.h"

namespace Minecraft
{
    CameraFrustum::CameraFrustum(mat4 projectionViewMatrix)
    {
        // Get the planes from the matrix
        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            planes[i] = glm::vec4(
                projectionViewMatrix[0][3] + (i == LEFT ? projectionViewMatrix[0][0] : 0.0f) + (i == RIGHT ? -projectionViewMatrix[0][0] : 0.0f),
                projectionViewMatrix[1][3] + (i == BOTTOM ? projectionViewMatrix[1][1] : 0.0f) + (i == TOP ? -projectionViewMatrix[1][1] : 0.0f),
                projectionViewMatrix[2][3] + (i == NEAR ? projectionViewMatrix[2][2] : 0.0f) + (i == FAR ? -projectionViewMatrix[2][2] : 0.0f),
                projectionViewMatrix[3][3]
            );

            // Normalize the planes
            planes[i] = glm::normalize(planes[i]);
        }
    }

    bool CameraFrustum::ContainsPoint(vec3 point) const
    {
        for (int i = 0; i < PLANE_COUNT; ++i)
        {
            if (glm::dot(glm::vec3(planes[i]), point) + planes[i].w < 0.0f)
            {
                return false;
            }
        }

        return true;
    }
}
