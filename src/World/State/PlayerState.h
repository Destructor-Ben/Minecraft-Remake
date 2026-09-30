#pragma once

#include "Graphics/Camera.h"
#include "Physics/RayHitInfo.h"
#include "World/Block.h"
#include "World/BlockType.h"

namespace Minecraft
{
    struct PlayerState
    {
        Transform PlayerTransform = { };
        ChunkPos CurrentChunkPos = { };
        ChunkPos PreviousChunkPos = { };
        bool HasPlayerMovedChunksThisTick = false;

        float FlySpeed = 7.5f;
        float FlySpeedFast = 15.0f;

        Camera PlayerCamera = { .FOV = 70.0f };
        float CameraPitch = 0.0f;
        float CameraYaw = 0.0f;
        float CameraSensitivity = 0.005f;
        float MaxCameraAngle = glm::radians(89.0f);

        float ReachDistance = 7.0f;
        optional<BlockType*> SelectedBlock = nullopt; // TODO: don't actually store a BlockType* here, rework BlockType so BlockType is an ID
        optional<RayHitInfo> TargetBlock = nullopt;
    };
}
