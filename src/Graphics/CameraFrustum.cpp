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

    // Thx to ChatGPT for explaining how these algorithms work and why planes are a 4D vector
    bool CameraFrustum::ContainsPoint(vec3 point) const
    {
        for (int i = 0; i < (int)CameraPlane::Count; ++i)
        {
            if (glm::dot(vec3(m_Planes[i]), point) + m_Planes[i].w < 0.0f)
                return false;
        }

        return true;
    }

    bool CameraFrustum::ContainsBounds(const BoundingBox& bounds)
    {
        for (const auto& plane : m_Planes)
        {
            // Compute the positive and negative vertices relative to the plane normal
            vec3 positiveVertex = bounds.Min;
            vec3 negativeVertex = bounds.Max;
            vec3 normal = vec3(plane);

            if (normal.x >= 0)
            {
                positiveVertex.x = bounds.Max.x;
                negativeVertex.x = bounds.Min.x;
            }

            if (normal.y >= 0)
            {
                positiveVertex.y = bounds.Max.y;
                negativeVertex.y = bounds.Min.y;
            }

            if (normal.z >= 0)
            {
                positiveVertex.z = bounds.Max.z;
                negativeVertex.z = bounds.Min.z;
            }

            // Check if the AABB is completely outside the plane
            if (glm::dot(normal, positiveVertex) + plane.w < 0)
                return false;
        }

        return true;
    }
}
