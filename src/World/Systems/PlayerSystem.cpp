#include "World/Systems/PlayerSystem.h"

#include "Input/Input.h"
#include "Physics/Physics.h"
#include "World/Blocks/Blocks.h"
#include "World/World.h"

namespace Minecraft::PlayerSystem
{
    static void TickMovement(PlayerState &player, float deltaTime);
    static void TickTargettedBlock(PlayerState &player, World &world);
    static void TickBlockBreakOrPlace(PlayerState &player, World &world);

    static void UpdateRotation(PlayerState &player);
    static void UpdateSelectedBlock(PlayerState &player);

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
        UpdateSelectedBlock(player);

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

        if (Input::WasMouseButtonPressed(MouseButton::Left))
        {
            auto targetBlock = world.GetBlock(targetBlockPos);
            if (!targetBlock.has_value())
                return;

            targetBlock->Data->Type = Blocks::Air;
            world.OnBlockModified(BlockPos(targetBlockPos));
        }
        else if (Input::WasMouseButtonPressed(MouseButton::Right) && player.SelectedBlock.has_value())
        {
            targetBlockPos.Pos += player.TargetBlock->HitFaceNormal;
            auto targetBlock = world.GetBlock(targetBlockPos);
            if (!targetBlock.has_value())
                return;

            targetBlock->Data->Type = Blocks::Air;
            world.OnBlockModified(BlockPos(targetBlockPos));

            if (targetBlock->Data->Type != Blocks::Air)
                return;

            targetBlock->Data->Type = player.SelectedBlock.value();
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

    static void UpdateSelectedBlock(PlayerState &player)
    {
        if (Input::WasKeyReleased(Key::Zero))
            player.SelectedBlock = nullopt;
        else if (Input::WasKeyReleased(Key::One))
            player.SelectedBlock = Blocks::Stone;
        else if (Input::WasKeyReleased(Key::Two))
            player.SelectedBlock = Blocks::Dirt;
        else if (Input::WasKeyReleased(Key::Three))
            player.SelectedBlock = Blocks::Grass;
        else if (Input::WasKeyReleased(Key::Four))
            player.SelectedBlock = Blocks::TallGrass;
        else if (Input::WasKeyReleased(Key::Five))
            player.SelectedBlock = Blocks::Sand;
        else if (Input::WasKeyReleased(Key::Six))
            player.SelectedBlock = Blocks::Clay;
        else if (Input::WasKeyReleased(Key::Seven))
            player.SelectedBlock = Blocks::IronOre;
        else if (Input::WasKeyReleased(Key::Eight))
            player.SelectedBlock = Blocks::Wood;
        else if (Input::WasKeyReleased(Key::Nine))
            player.SelectedBlock = Blocks::Leaves;
    }
}
