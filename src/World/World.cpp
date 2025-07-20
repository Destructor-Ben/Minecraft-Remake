#include "World.h"

#include "Game.h"
#include "Logger.h"
#include "Profiler.h"
#include "Input/Input.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/SkyRenderer.h"
#include "World/Chunk.h"
#include "Physics/Physics.h"

namespace Minecraft
{
    World::World(ulong seed)
    {
        m_Seed = seed;
        m_WorldGenerator = WorldGenerator(this, seed);
    }

    void World::Generate()
    {
        m_WorldGenerator.Generate(SpawnRadius, MinSpawnHeight, MaxSpawnHeight);

        // TODO: Reset time now since it will take time for the world to generate
        // Time = Dawn;
    }

    void World::OnEnter()
    {
        PlayerCamera.FOV = 70.0f;

        Instance->Graphics->OnEnterWorld();
    }

    void World::OnExit()
    {
        Instance->Graphics->OnExitWorld();
    }

    void World::Tick()
    {
        Instance->PerfProfiler->Push("World::Tick");

        UpdateChunkList(m_LoadedChunks, SimulationDistance);

        Instance->PerfProfiler->Push("World::TickPlayer");
        UpdateCamera();
        HasPlayerMovedChunks = ChunkPos::FromWorldPos(PlayerCamera.Position) != PreviousPlayerChunkPos;
        PlayerCamera.Update();
        UpdateBlockBreaking();
        Instance->PerfProfiler->Pop();

        TickTime();

        Instance->PerfProfiler->Pop();
    }

    void World::Update()
    {
        Instance->PerfProfiler->Push("World::Update");

        UpdateChunkList(m_RenderedChunks, RenderDistance);

        m_WorldGenerator.GenerateChunksAroundPlayer(PlayerCamera.Position, GenerationDistance, MinHeight, MaxHeight);

        Instance->SkyGraphics->Update();

        Instance->PerfProfiler->Pop();
    }

    void World::Render()
    {
        Instance->PerfProfiler->Push("World::Render");

        Instance->Graphics->SceneCamera = &PlayerCamera;

        Instance->ChunkGraphics->RenderChunks(GetRenderedChunks());

        if (Instance->ChunkGraphics->DrawChunkBorders)
            Instance->ChunkGraphics->RenderDebugChunkBorders();

        Instance->SkyGraphics->Render();

        Instance->PerfProfiler->Pop();
    }

    optional<Chunk*> World::GetChunk(const ChunkPos& chunkPos)
    {
        if (!Chunks.contains(chunkPos))
            return nullopt;

        return &Chunks.at(chunkPos);
    }

    optional <Block> World::GetBlock(const BlockPos& pos)
    {
        // Calculate coordinates
        ChunkPos chunkPos = ChunkPos::FromBlockPos(pos);
        BlockOffset blockOffset = BlockOffset::FromBlockPos(pos);

        // Get block from chunk
        auto chunk = GetChunk(chunkPos);
        if (!chunk.has_value())
            return nullopt;

        return chunk.value()->GetBlock(blockOffset);
    }

    void World::UpdateChunkList(vector<Chunk*>& chunks, int radius)
    {
        Instance->PerfProfiler->Push("World::UpdateChunkList");

        // Only refresh chunks when moving along chunk borders
        if (!HasPlayerMovedChunks)
        {
            Instance->PerfProfiler->Pop();
            return;
        }

        auto playerChunkPos = ChunkPos::FromWorldPos(PlayerCamera.Position);
        chunks.clear();

        for_chunk_in_radius(x, y, z, radius, {
            // Calculate chunk pos
            auto chunkPos = ChunkPos(x, y, z);
            chunkPos.Pos += playerChunkPos.Pos;

            // Add the chunk
            auto chunk = GetChunk(chunkPos);
            if (!chunk.has_value())
                continue;

            chunks.push_back(chunk.value());
        })

        Instance->PerfProfiler->Pop();
    }

    void World::TickTime()
    {
        Time += Instance->TickDeltaTime;

        // New days start after dawn, not midnight
        if (Time >= DayLength)
        {
            Time -= DayLength;
            DayCount++;
        }

        TimePercent = Time / DayLength;
        IsDay = Time > Dawn && Time < Dusk;
    }

    void World::UpdateCamera()
    {
        PreviousPlayerChunkPos = ChunkPos::FromWorldPos(PlayerCamera.Position);

        const float sensitivity = 0.005f;
        const float maxAngle = glm::radians(89.0f);
        float speed = 12.5f * Instance->DeltaTime;

        // Rotation
        m_CameraPitch += Input::GetMousePosDelta().y * sensitivity;
        m_CameraYaw -= Input::GetMousePosDelta().x * sensitivity;
        m_CameraPitch = glm::clamp(m_CameraPitch, -maxAngle, maxAngle);
        PlayerCamera.Rotation = quat(vec3(m_CameraPitch, m_CameraYaw, 0.0f));

        // Input
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

        // Vertical movement
        PlayerCamera.Position.y += movementDirection.y * speed;

        // Horizontal movement
        if (movementDirection.x != 0 || movementDirection.z != 0)
        {
            // Normalising horizontal movement direction
            vec2 horizontalDirection = glm::normalize(vec2(movementDirection.x, movementDirection.z));
            movementDirection.x = horizontalDirection.x;
            movementDirection.z = horizontalDirection.y;

            // Calculating forward and right vectors
            vec3 cameraForward = PlayerCamera.GetForwardVector();
            vec3 cameraRight = PlayerCamera.GetRightVector();

            // Disable movement on the Y axis from WASD movement
            cameraForward.y = 0.0f;
            cameraForward = glm::normalize(cameraForward);

            cameraRight.y = 0.0f;
            cameraRight = glm::normalize(cameraRight);

            // Moving camera
            PlayerCamera.Position += cameraForward * -movementDirection.z * speed; // There is a negative sign here because movement direction -z is forward, but camera forward -z backwards
            PlayerCamera.Position += cameraRight * movementDirection.x * speed;
        }
    }

    void World::UpdateBlockBreaking()
    {
        // Block selecting
        if (Input::WasKeyReleased(Key::Zero))
            SelectedBlock = nullptr;
        else if (Input::WasKeyReleased(Key::One))
            SelectedBlock = Blocks::Stone;
        else if (Input::WasKeyReleased(Key::Two))
            SelectedBlock = Blocks::Dirt;
        else if (Input::WasKeyReleased(Key::Three))
            SelectedBlock = Blocks::Grass;
        else if (Input::WasKeyReleased(Key::Four))
            SelectedBlock = Blocks::TallGrass;
        else if (Input::WasKeyReleased(Key::Five))
            SelectedBlock = Blocks::Sand;
        else if (Input::WasKeyReleased(Key::Six))
            SelectedBlock = Blocks::Clay;
        else if (Input::WasKeyReleased(Key::Seven))
            SelectedBlock = Blocks::IronOre;
        else if (Input::WasKeyReleased(Key::Eight))
            SelectedBlock = Blocks::Wood;
        else if (Input::WasKeyReleased(Key::Nine))
            SelectedBlock = Blocks::Leaves;

        // Block breaking
        auto ray = Physics::RaycastBlocks(PlayerCamera.Position, PlayerCamera.GetForwardVector(), PlayerReachDistance);
        if (ray.DidHit)
            PlayerTargetBlock = GetBlock(BlockPos::FromWorldPos(ray.HitBlockPos));
        else
            PlayerTargetBlock = nullopt;

        if (!PlayerTargetBlock.has_value())
            return;

        auto targetBlock = PlayerTargetBlock;
        if (!targetBlock.has_value())
            return;

        // TODO: split these into more functions
        // Block breaking
        if (Input::WasMouseButtonPressed(MouseButton::Left))
        {
            targetBlock->Data->Type = Blocks::Air;
            // TODO: priority
            // TODO: make a function to update meshes at a block position if it gets modified
            Instance->ChunkGraphics->QueueMeshRegen(*targetBlock->GetChunk());

            // Regen adjacent chunk meshes
            auto chunkPos = targetBlock->GetChunk()->GetChunkPos();
            auto blockPos = targetBlock->GetBlockPos();

            if (blockPos.x == 0)
                UpdateMeshInDirection(chunkPos, vec3i(-1, 0, 0));

            if (blockPos.y == 0)
                UpdateMeshInDirection(chunkPos, vec3i(0, -1, 0));

            if (blockPos.z == 0)
                UpdateMeshInDirection(chunkPos, vec3i(0, 0, -1));

            if (blockPos.x == Chunk::Size - 1)
                UpdateMeshInDirection(chunkPos, vec3i(1, 0, 0));

            if (blockPos.y == Chunk::Size - 1)
                UpdateMeshInDirection(chunkPos, vec3i(0, 1, 0));

            if (blockPos.z == Chunk::Size - 1)
                UpdateMeshInDirection(chunkPos, vec3i(0, 0, 1));

            return;
        }

        // Block placing
        // TODO: what if place + break in the same tick?
        if (Input::WasMouseButtonPressed(MouseButton::Right) && SelectedBlock != nullptr)
        {
            auto placedBlockPos = PlayerTargetBlock.value().GetBlockPos();
            placedBlockPos.Pos += ray.HitFaceNormal;
            auto placedBlock = GetBlock(placedBlockPos);
            if (!placedBlock.has_value())
                return;

            if (placedBlock->Data->Type != Blocks::Air)
                return;

            placedBlock->Data->Type = SelectedBlock;
            // TODO: priority
            Instance->ChunkGraphics->QueueMeshRegen(*placedBlock->GetChunk());

            // Regen adjacent chunk meshes
            // TODO: check this is correct for the placed block
            auto chunkPos = placedBlock->GetChunk()->GetChunkPos();
            auto blockPos = placedBlock->GetBlockPos();

            if (blockPos.x == 0)
                UpdateMeshInDirection(chunkPos, vec3i(-1, 0, 0));

            if (blockPos.y == 0)
                UpdateMeshInDirection(chunkPos, vec3i(0, -1, 0));

            if (blockPos.z == 0)
                UpdateMeshInDirection(chunkPos, vec3i(0, 0, -1));

            if (blockPos.x == Chunk::Size - 1)
                UpdateMeshInDirection(chunkPos, vec3i(1, 0, 0));

            if (blockPos.y == Chunk::Size - 1)
                UpdateMeshInDirection(chunkPos, vec3i(0, 1, 0));

            if (blockPos.z == Chunk::Size - 1)
                UpdateMeshInDirection(chunkPos, vec3i(0, 0, 1));
        }
    }

    void World::UpdateMeshInDirection(const ChunkPos& chunkPos, vec3i dir)
    {
        auto newChunkPos = chunkPos;
        newChunkPos.Pos += dir;
        auto chunk = GetChunk(newChunkPos);
        if (chunk.has_value())
        {
            // TODO: priority
            Instance->ChunkGraphics->QueueMeshRegen(*chunk.value());
        }
    }
}
