#include "World.h"

#include "Game.h"
#include "Logger.h"
#include "Profiler.h"
#include "Input/InputManager.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/SkyRenderer.h"
#include "World/Chunk.h"
#include "Physics/Physics.h"

namespace Minecraft
{
    #pragma region Coordinate Conversion Functions

    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "ConstantParameter"

    static int FloorDivision(int x, int y)
    {
        return (x < 0) ? ((x - y + 1) / y) : (x / y);
    }

    static int Modulus(int x, int y)
    {
        return (x % y + y) % y;
    }

    #pragma clang diagnostic pop

    // TODO: Chunk::Size can't be used here for some reason, the linker whines about it

    vec3i WorldToChunkPos(vec3 pos)
    {
        return vec3i(
            FloorDivision((int)std::floor(pos.x), Chunk::Size),
            FloorDivision((int)std::floor(pos.y), 10),
            FloorDivision((int)std::floor(pos.z), 10)
        );
    }

    vec3i WorldToBlockPos(vec3 pos)
    {
        return vec3i(
            Modulus((int)std::floor(pos.x), 10),
            Modulus((int)std::floor(pos.y), 10),
            Modulus((int)std::floor(pos.z), 10)
        );
    }

    vec3 ChunkToWorldPos(vec3i chunkPos)
    {
        return vec3(chunkPos) * 10;
    }

    vec3 BlockAndChunkToWorldPos(vec3i chunkPos, vec3i blockPos)
    {
        return ChunkToWorldPos(chunkPos) + vec3(blockPos);
    }

    #pragma endregion

    World::World()
    {
        PlayerCamera.FOV = 70.0f;

        // TODO: random seed generation
        m_WorldGenerator = WorldGenerator(this);
        m_WorldGenerator.Generate(SpawnRadius, MinSpawnHeight, MaxSpawnHeight);
    }

    void World::Tick()
    {
        Instance->PerfProfiler->Push("World::Tick");

        UpdateChunkList(m_LoadedChunks, SimulationDistance);

        Instance->PerfProfiler->Push("World::TickChunks");
        for (auto* chunk : GetLoadedChunks())
        {
            chunk->Tick();
        }
        Instance->PerfProfiler->Pop();

        Instance->PerfProfiler->Push("World::TickPlayer");
        UpdateCamera();
        HasPlayerMovedChunks = WorldToChunkPos(PlayerCamera.Position) != PreviousPlayerChunkPos;
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

        Instance->PerfProfiler->Push("World::UpdateChunks");
        for (auto* chunk : GetRenderedChunks())
        {
            chunk->Update();
        }
        Instance->PerfProfiler->Pop();

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

    optional<Chunk*> World::GetChunk(vec3i chunkPos)
    {
        if (!Chunks.contains(chunkPos))
            return nullopt;

        return &Chunks.at(chunkPos);
    }

    optional <Block> World::GetBlock(vec3i pos)
    {
        // Calculate coordinates
        vec3i chunkPos = WorldToChunkPos(pos);
        vec3i blockPos = WorldToBlockPos(pos);

        // Get block from chunk
        auto chunk = GetChunk(chunkPos);
        if (!chunk.has_value())
            return nullopt;

        return chunk.value()->GetBlock(blockPos);
    }

    void World::UpdateChunkList(vector<Chunk*>& chunks, int radius)
    {
        Instance->PerfProfiler->Push("World::UpdateChunkList");

        // Only refresh chunks when moving along chunk borders
        if (HasPlayerMovedChunks)
        {
            Instance->PerfProfiler->Pop();
            return;
        }

        auto playerChunkPos = WorldToChunkPos(PlayerCamera.Position);
        chunks.clear();

        for_chunk_in_radius(x, y, z, radius, {
            // Calculate chunk pos
            auto chunkPos = vec3i(x, y, z);
            chunkPos += playerChunkPos;

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
        PreviousPlayerChunkPos = WorldToChunkPos(PlayerCamera.Position);

        const float sensitivity = 0.005f;
        const float maxAngle = glm::radians(89.0f);
        float speed = 12.5f * Instance->DeltaTime;

        // Rotation
        m_CameraPitch += Instance->Input->GetMousePosDelta().y * sensitivity;
        m_CameraYaw -= Instance->Input->GetMousePosDelta().x * sensitivity;
        m_CameraPitch = glm::clamp(m_CameraPitch, -maxAngle, maxAngle);
        PlayerCamera.Rotation = quat(vec3(m_CameraPitch, m_CameraYaw, 0.0f));

        // Input
        vec3 movementDirection = vec3(0.0f);

        if (Instance->Input->IsKeyDown(Key::W))
            movementDirection.z -= 1;

        if (Instance->Input->IsKeyDown(Key::S))
            movementDirection.z += 1;

        if (Instance->Input->IsKeyDown(Key::A))
            movementDirection.x -= 1;

        if (Instance->Input->IsKeyDown(Key::D))
            movementDirection.x += 1;

        if (Instance->Input->IsKeyDown(Key::Space))
            movementDirection.y += 1;

        if (Instance->Input->IsKeyDown(Key::LeftShift))
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
        // Block breaking
        auto ray = Physics::RaycastBlocks(PlayerCamera.Position, PlayerCamera.GetForwardVector(), PlayerReachDistance);
        if (ray.DidHit)
            PlayerTargetedBlockPos = ray.HitBlockPos;
        else
            PlayerTargetedBlockPos = nullopt;

        if (!PlayerTargetedBlockPos.has_value())
            return;

        auto targetBlock = GetBlock(PlayerTargetedBlockPos.value());
        if (!targetBlock.has_value())
            return;

        // TODO: split these into more functions
        // Block breaking
        if (Instance->Input->WasMouseButtonPressed(MouseButton::Left))
        {
            targetBlock->Data.Type = Blocks::Air;
            // TODO: priority
            // TODO: make a function to update meshes at a block position if it gets modified
            Instance->ChunkGraphics->QueueMeshRegen(targetBlock->GetChunk());

            // Regen adjacent chunk meshes
            auto chunkPos = targetBlock->GetChunk().GetChunkPos();
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
        // TODO: wonky
        if (Instance->Input->WasMouseButtonPressed(MouseButton::Right))
        {
            auto placedBlockPos = PlayerTargetedBlockPos.value() + ray.HitFaceNormal;
            auto placedBlock = GetBlock(placedBlockPos);
            if (!placedBlock.has_value())
                return;

            if (placedBlock->Data.Type != Blocks::Air)
                return;

            // TODO: allow selecting the block type
            placedBlock->Data.Type = Blocks::Sand;
            // TODO: priority
            Instance->ChunkGraphics->QueueMeshRegen(placedBlock->GetChunk());

            // Regen adjacent chunk meshes
            // TODO: check this is correct for the placed block
            auto chunkPos = placedBlock->GetChunk().GetChunkPos();
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

    void World::UpdateMeshInDirection(vec3i chunkPos, vec3i dir)
    {
        auto chunk = GetChunk(chunkPos + dir);
        if (chunk.has_value())
        {
            // TODO: priority
            Instance->ChunkGraphics->QueueMeshRegen(*chunk.value());
        }
    }
}
