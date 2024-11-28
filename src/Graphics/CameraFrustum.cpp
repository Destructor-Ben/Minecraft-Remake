#include "CameraFrustum.h"

#include "BoundingBox.h"

namespace Minecraft
{
    CameraFrustum::CameraFrustum(mat4 projectionViewMatrix)
    {
        // Left Plane
        m_Planes[(int)CameraPlane::Left] = glm::vec4(
            projectionViewMatrix[0][3] + projectionViewMatrix[0][0],
            projectionViewMatrix[1][3] + projectionViewMatrix[1][0],
            projectionViewMatrix[2][3] + projectionViewMatrix[2][0],
            projectionViewMatrix[3][3] + projectionViewMatrix[3][0]
        );

        // Right Plane
        m_Planes[(int)CameraPlane::Right] = glm::vec4(
            projectionViewMatrix[0][3] - projectionViewMatrix[0][0],
            projectionViewMatrix[1][3] - projectionViewMatrix[1][0],
            projectionViewMatrix[2][3] - projectionViewMatrix[2][0],
            projectionViewMatrix[3][3] - projectionViewMatrix[3][0]
        );

        // Bottom Plane
        m_Planes[(int)CameraPlane::Bottom] = glm::vec4(
            projectionViewMatrix[0][3] + projectionViewMatrix[0][1],
            projectionViewMatrix[1][3] + projectionViewMatrix[1][1],
            projectionViewMatrix[2][3] + projectionViewMatrix[2][1],
            projectionViewMatrix[3][3] + projectionViewMatrix[3][1]
        );

        // Top Plane
        m_Planes[(int)CameraPlane::Top] = glm::vec4(
            projectionViewMatrix[0][3] - projectionViewMatrix[0][1],
            projectionViewMatrix[1][3] - projectionViewMatrix[1][1],
            projectionViewMatrix[2][3] - projectionViewMatrix[2][1],
            projectionViewMatrix[3][3] - projectionViewMatrix[3][1]
        );

        // Near Plane
        m_Planes[(int)CameraPlane::Near] = glm::vec4(
            projectionViewMatrix[0][3] + projectionViewMatrix[0][2],
            projectionViewMatrix[1][3] + projectionViewMatrix[1][2],
            projectionViewMatrix[2][3] + projectionViewMatrix[2][2],
            projectionViewMatrix[3][3] + projectionViewMatrix[3][2]
        );

        // Far Plane
        m_Planes[(int)CameraPlane::Far] = glm::vec4(
            projectionViewMatrix[0][3] - projectionViewMatrix[0][2],
            projectionViewMatrix[1][3] - projectionViewMatrix[1][2],
            projectionViewMatrix[2][3] - projectionViewMatrix[2][2],
            projectionViewMatrix[3][3] - projectionViewMatrix[3][2]
        );

        // Normalize the planes
        for (int i = 0; i < (int)CameraPlane::Count; ++i)
        {
            float length = glm::length(glm::vec3(m_Planes[i]));
            m_Planes[i] /= length; // Normalize plane coefficients
        }
    }

    bool CameraFrustum::ContainsPoint(vec3 point) const
    {
        for (int i = 0; i < (int)CameraPlane::Count; ++i)
        {
            if (glm::dot(glm::vec3(m_Planes[i]), point) + m_Planes[i].w < 0.0f)
            {
                return false;
            }
        }

        return true;
    }

    bool CameraFrustum::ContainsBounds(const BoundingBox& bounds)
    {
        vec3 min = bounds.Origin;
        vec3 max = bounds.Origin + bounds.Size;

        // Iterate over all 8 corners of the bounding box
        for (int x = 0; x < 2; ++x)
        {
            for (int y = 0; y < 2; ++y)
            {
                for (int z = 0; z < 2; ++z)
                {
                    vec3 corner = {
                        x == 0 ? min.x : max.x,
                        y == 0 ? min.y : max.y,
                        z == 0 ? min.z : max.z
                    };

                    // If any corner is inside, return true
                    if (ContainsPoint(corner))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }
}
