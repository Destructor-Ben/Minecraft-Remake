#pragma once

namespace Minecraft
{
    struct RayHitInfo
    {
        bool DidHit = false;
        vec3 HitPos = { };
        vec3i HitBlockPos = { };
        vec3i HitFaceNormal = { };
        float Length = 0;
    };
}
