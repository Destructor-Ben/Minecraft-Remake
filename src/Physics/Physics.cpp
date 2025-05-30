#include "Physics.h"

#include "Game.h"
#include "World/World.h"

namespace Minecraft::Physics
{
    // Amanatides-Woo algorithm
    RayHitInfo RaycastBlocks(const vec3& rayOrigin, const vec3& rayDir, float maxDistance)
    {
        auto result = RayHitInfo();
        vec3 dir = glm::normalize(rayDir);
        vec3i voxelPos = glm::floor(rayOrigin);
        float maxFloat = std::numeric_limits<float>::max();

        vec3i step = glm::ivec3(
            dir.x >= 0 ? 1 : -1,
            dir.y >= 0 ? 1 : -1,
            dir.z >= 0 ? 1 : -1
        );

        vec3 tDelta = glm::vec3(
            dir.x != 0 ? 1.0f / std::abs(dir.x) : maxFloat,
            dir.y != 0 ? 1.0f / std::abs(dir.y) : maxFloat,
            dir.z != 0 ? 1.0f / std::abs(dir.z) : maxFloat
        );

        vec3 tMax = glm::vec3(
            dir.x != 0 ? (voxelPos.x + (dir.x >= 0 ? 1 : 0) - rayOrigin.x) / dir.x : maxFloat,
            dir.y != 0 ? (voxelPos.y + (dir.y >= 0 ? 1 : 0) - rayOrigin.y) / dir.y : maxFloat,
            dir.z != 0 ? (voxelPos.z + (dir.z >= 0 ? 1 : 0) - rayOrigin.z) / dir.z : maxFloat
        );

        // Distance along the ray
        float t = 0.0f;

        while (t < maxDistance)
        {
            // Check if the current voxel is a solid block
            auto block = Instance->CurrentWorld->GetBlock(voxelPos);
            if (block.has_value() && block->Data.Type != Blocks::Air)
            {
                result.DidHit = true;
                result.HitPos = rayOrigin + t * dir;
                result.HitBlockPos = voxelPos;
                result.Length = t;

                // Determine which face was hit based on the axis with smallest tMax
                float minTMax = std::min({ tMax.x, tMax.y, tMax.z });
                if (minTMax == tMax.x)
                    result.HitFaceNormal = vec3i(-step.x, 0, 0);
                else if (minTMax == tMax.y)
                    result.HitFaceNormal = vec3i(0, -step.y, 0);
                else
                    result.HitFaceNormal = vec3i(0, 0, -step.z);

                return result;
            }

            // Advance to the next voxel
            if (tMax.x < tMax.y && tMax.x < tMax.z)
            {
                voxelPos.x += step.x;
                t = tMax.x;
                tMax.x += tDelta.x;
            }
            else if (tMax.y < tMax.z)
            {
                voxelPos.y += step.y;
                t = tMax.y;
                tMax.y += tDelta.y;
            }
            else
            {
                voxelPos.z += step.z;
                t = tMax.z;
                tMax.z += tDelta.z;
            }
        }

        // No hit was made
        return result;
    }
}
