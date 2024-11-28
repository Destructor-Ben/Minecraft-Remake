#include "CameraFrustum.h"

#include "BoundingBox.h"

namespace Minecraft
{
    CameraFrustum::CameraFrustum(mat4 projectionViewMatrix)
    {
        // Get the planes from the matrix
        for (int i = 0; i < (int)CameraPlane::Count; ++i)
        {
            // TODO: this might be wrong
            m_Planes[i] = glm::vec4(
                projectionViewMatrix[0][3] + (i == (int)CameraPlane::Left ? projectionViewMatrix[0][0] : 0.0f) + (i == (int)CameraPlane::Right ? -projectionViewMatrix[0][0] : 0.0f),
                projectionViewMatrix[1][3] + (i == (int)CameraPlane::Bottom ? projectionViewMatrix[1][1] : 0.0f) + (i == (int)CameraPlane::Top ? -projectionViewMatrix[1][1] : 0.0f),
                projectionViewMatrix[2][3] + (i == (int)CameraPlane::Near ? projectionViewMatrix[2][2] : 0.0f) + (i == (int)CameraPlane::Far ? -projectionViewMatrix[2][2] : 0.0f),
                projectionViewMatrix[3][3]
            );

            // Normalize the planes
            m_Planes[i] = glm::normalize(m_Planes[i]);
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
