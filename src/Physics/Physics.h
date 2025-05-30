#pragma once

#include "Physics/RayHitInfo.h"

namespace Minecraft::Physics
{
    RayHitInfo RaycastBlocks(const vec3& rayOrigin, const vec3& rayDir, float maxDistance);
}
