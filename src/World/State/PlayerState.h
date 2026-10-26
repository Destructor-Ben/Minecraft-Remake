#pragma once

#include "Graphics/Camera.h"
#include "Physics/RayHitInfo.h"
#include "World/Block.h"
#include "World/BlockType.h"

namespace Minecraft
{
    // TODO: move some of the constants to a config
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
        optional<RayHitInfo> TargetBlock = nullopt;

        // Will not be made into a config option because I hate how Minecraft has 9 hotbar slots (10 leads to nice round numbers)
        static constexpr int HotbarSize = 10;

        int SelectedHotbarSlot = 0;
        // TODO: in future, make this an array of ItemStack
        array<optional<BlockType*>, HotbarSize> Hotbar = { };

        optional<BlockType*> GetSelectedItem()
        {
            return Hotbar[SelectedHotbarSlot];
        }
    };
}
