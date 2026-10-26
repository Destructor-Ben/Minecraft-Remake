#include "World/Systems/PlayerSystem.h"

#include "Input/Input.h"
#include "Logger.h"
#include "Physics/Physics.h"
#include "World/Blocks/Blocks.h"
#include "World/World.h"

namespace Minecraft::PlayerSystem
{
    static void TickMovement(PlayerState &player, float deltaTime);
    static void TickTargettedBlock(PlayerState &player, World &world);
    static void TickBlockBreakOrPlace(PlayerState &player, World &world);

    static void UpdateRotation(PlayerState &player);
    static void UpdateHotbar(PlayerState &player);

    void Tick(PlayerState &player, World &world, float deltaTime)
    {
        TickMovement(player, deltaTime);
        TickTargettedBlock(player, world);
        TickBlockBreakOrPlace(player, world);

        player.PreviousChunkPos = player.CurrentChunkPos;
        player.CurrentChunkPos = ChunkPos::FromWorldPos(player.PlayerTransform.Position);
        player.HasPlayerMovedChunksThisTick = player.CurrentChunkPos != player.PreviousChunkPos;
    }

    void Update(PlayerState &player, int screenWidth, int screenHeight)
    {
        UpdateRotation(player);
        UpdateHotbar(player);

        player.PlayerCamera.ViewTransform = player.PlayerTransform;
        player.PlayerCamera.Update(screenWidth, screenHeight);
    }

    void OnEnterWorld(PlayerState &player)
    {
        vec3 initialPlayerPos = vec3(0, 10.0f, 0);
        player.PlayerTransform = { .Position = initialPlayerPos };
        player.CameraYaw = glm::radians(-90.0f);
        player.CameraPitch = glm::radians(5.0f);
        player.CurrentChunkPos = ChunkPos::FromWorldPos(initialPlayerPos);
        player.PreviousChunkPos = player.CurrentChunkPos;
        player.HasPlayerMovedChunksThisTick = true;

        // TODO: temporary until I make a proper inventory system
        player.Hotbar[0] = nullopt;
        player.Hotbar[1] = Blocks::Grass;
        player.Hotbar[2] = Blocks::Dirt;
        player.Hotbar[3] = Blocks::Stone;
        player.Hotbar[4] = Blocks::TallGrass;
        player.Hotbar[5] = Blocks::Sand;
        player.Hotbar[6] = Blocks::Gravel;
        player.Hotbar[7] = Blocks::Clay;
        player.Hotbar[8] = Blocks::Wood;
        player.Hotbar[9] = Blocks::Leaves;
    }

    static void TickMovement(PlayerState &player, float deltaTime)
    {
        float speed = (Input::IsKeyDown(Key::LeftControl) ? player.FlySpeedFast : player.FlySpeed) * deltaTime;

        vec3 movementDirection = vec3(0.0f);

        if (Input::IsKeyDown(Key::W))
            movementDirection.z -= 1;

        if (Input::IsKeyDown(Key::S))
            movementDirection.z += 1;

        if (Input::IsKeyDown(Key::A))
            movementDirection.x -= 1;

        if (Input::IsKeyDown(Key::D))
            movementDirection.x += 1;

        if (Input::IsKeyDown(Key::Space))
            movementDirection.y += 1;

        if (Input::IsKeyDown(Key::LeftShift))
            movementDirection.y -= 1;

        player.PlayerTransform.Position.y += movementDirection.y * speed;

        if (movementDirection.x == 0 && movementDirection.z == 0)
            return;

        vec2 horizontalDirection = glm::normalize(vec2(movementDirection.x, movementDirection.z));
        movementDirection.x = horizontalDirection.x;
        movementDirection.z = horizontalDirection.y;

        vec3 cameraForward = player.PlayerTransform.GetForwardVector();
        vec3 cameraRight = player.PlayerTransform.GetRightVector();

        // Disable movement on the Y axis from WASD movement
        cameraForward.y = 0.0f;
        cameraForward = glm::normalize(cameraForward);
        cameraRight.y = 0.0f;
        cameraRight = glm::normalize(cameraRight);

        player.PlayerTransform.Position -= cameraForward * movementDirection.z * speed;
        player.PlayerTransform.Position += cameraRight * movementDirection.x * speed;
    }

    static void TickTargettedBlock(PlayerState &player, World &world)
    {
        auto ray = Physics::RaycastBlocks(player.PlayerTransform.Position, player.PlayerTransform.GetForwardVector(), player.ReachDistance);

        if (ray.DidHit)
            player.TargetBlock = ray;
        else
            player.TargetBlock = nullopt;
    }

    static void TickBlockBreakOrPlace(PlayerState &player, World &world)
    {
        if (!player.TargetBlock.has_value())
            return;

        auto targetBlockPos = BlockPos(player.TargetBlock->HitBlockPos);
        auto selectedItem = player.GetSelectedItem();

        if (Input::WasMouseButtonPressed(MouseButton::Left))
        {
            auto targetBlock = world.GetBlock(targetBlockPos);
            if (!targetBlock.has_value())
                return;

            targetBlock->Data->Type = Blocks::Air;
            world.OnBlockModified(BlockPos(targetBlockPos));
        }
        else if (Input::WasMouseButtonPressed(MouseButton::Right) && selectedItem.has_value())
        {
            targetBlockPos.Pos += player.TargetBlock->HitFaceNormal;
            auto targetBlock = world.GetBlock(targetBlockPos);
            if (!targetBlock.has_value())
                return;

            targetBlock->Data->Type = Blocks::Air;
            world.OnBlockModified(BlockPos(targetBlockPos));

            if (targetBlock->Data->Type != Blocks::Air)
                return;

            targetBlock->Data->Type = selectedItem.value();
            world.OnBlockModified(targetBlockPos);
        }
    }

    static void UpdateRotation(PlayerState &player)
    {
        player.CameraPitch += Input::GetMousePosDelta().y * player.CameraSensitivity;
        player.CameraYaw -= Input::GetMousePosDelta().x * player.CameraSensitivity;
        player.CameraPitch = glm::clamp(player.CameraPitch, -player.MaxCameraAngle, player.MaxCameraAngle);
        player.PlayerTransform.Rotation = quat(vec3(player.CameraPitch, player.CameraYaw, 0.0f));
    }

    static void UpdateHotbar(PlayerState &player)
    {
        float scrollDelta = Input::GetScrollWheelDelta();
        if (scrollDelta != 0)
        {
            player.SelectedHotbarSlot -= (int)scrollDelta;

            // Don't use modulus, since it is machine dependent for negatives
            while (player.SelectedHotbarSlot >= PlayerState::HotbarSize)
            {
                player.SelectedHotbarSlot -= PlayerState::HotbarSize;
            }

            while (player.SelectedHotbarSlot < 0)
            {
                player.SelectedHotbarSlot += PlayerState::HotbarSize;
            }
        }

        if (Input::WasKeyReleased(Key::One))
            player.SelectedHotbarSlot = 0;
        else if (Input::WasKeyReleased(Key::Two))
            player.SelectedHotbarSlot = 1;
        else if (Input::WasKeyReleased(Key::Three))
            player.SelectedHotbarSlot = 2;
        else if (Input::WasKeyReleased(Key::Four))
            player.SelectedHotbarSlot = 3;
        else if (Input::WasKeyReleased(Key::Five))
            player.SelectedHotbarSlot = 4;
        else if (Input::WasKeyReleased(Key::Six))
            player.SelectedHotbarSlot = 5;
        else if (Input::WasKeyReleased(Key::Seven))
            player.SelectedHotbarSlot = 6;
        else if (Input::WasKeyReleased(Key::Eight))
            player.SelectedHotbarSlot = 7;
        else if (Input::WasKeyReleased(Key::Nine))
            player.SelectedHotbarSlot = 8;
        else if (Input::WasKeyReleased(Key::Zero))
            player.SelectedHotbarSlot = 9;
    }
}
