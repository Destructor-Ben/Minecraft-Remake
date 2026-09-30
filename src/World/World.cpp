#include "World.h"

#include "Game.h"
#include "Logger.h"
#include "Profiler.h"
#include "Input/Input.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/SkyRenderer.h"
#include "Transform.h"
#include "World/Chunk.h"
#include "Physics/Physics.h"

#include "World/Coords.h"
#include "World/State/SkyState.h"
#include "World/Systems/PlayerSystem.h"
#include "World/Systems/SkySystem.h"
#include "World/Systems/TimeSystem.h"

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
        PlayerSystem::OnEnterWorld(Player);

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

        Instance->PerfProfiler->Push("PlayerSystem::Tick");
        PlayerSystem::Tick(Player, *this, Instance->TickDeltaTime);
        Instance->PerfProfiler->Pop();

        TimeSystem::Tick(Time, Instance->TickDeltaTime);

        Instance->PerfProfiler->Pop();
    }

    void World::Update()
    {
        Instance->PerfProfiler->Push("World::Update");

        UpdateChunkList(m_RenderedChunks, RenderDistance);

        PlayerSystem::Update(Player, Instance->ScreenWidth, Instance->ScreenHeight);
        SkySystem::Update(Sky, Time);

        // TODO: this should be in Tick
        m_WorldGenerator.GenerateChunksAroundPlayer(Player.PlayerTransform.Position, GenerationDistance, MinHeight, MaxHeight);

        Instance->SkyGraphics->Update(Sky);

        Instance->PerfProfiler->Pop();
    }

    void World::Render()
    {
        Instance->PerfProfiler->Push("World::Render");

        Instance->Graphics->SceneCamera = &Player.PlayerCamera;

        Instance->ChunkGraphics->RenderChunks(GetRenderedChunks(), Sky);

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

    // Handle this based on "Chunk Loaders" which are entities/blocks that have the "chunk loader" property
    void World::UpdateChunkList(vector<Chunk*>& chunks, int radius)
    {
        Instance->PerfProfiler->Push("World::UpdateChunkList");

        // Only refresh chunks when moving along chunk borders
        if (!Player.HasPlayerMovedChunksThisTick)
        {
            Instance->PerfProfiler->Pop();
            return;
        }

        auto playerChunkPos = ChunkPos::FromWorldPos(Player.PlayerTransform.Position);
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

    void World::OnBlockModified(const BlockPos &pos)
    {
        auto chunkPos = ChunkPos::FromBlockPos(pos);
        auto blockOffset = BlockOffset::FromBlockPos(pos);

        UpdateMeshInDirection(chunkPos, vec3i(0, 0, 0));

        if (blockOffset.x == 0)
            UpdateMeshInDirection(chunkPos, vec3i(-1, 0, 0));

        if (blockOffset.y == 0)
            UpdateMeshInDirection(chunkPos, vec3i(0, -1, 0));

        if (blockOffset.z == 0)
            UpdateMeshInDirection(chunkPos, vec3i(0, 0, -1));

        if (blockOffset.x == Chunk::Size - 1)
            UpdateMeshInDirection(chunkPos, vec3i(1, 0, 0));

        if (blockOffset.y == Chunk::Size - 1)
            UpdateMeshInDirection(chunkPos, vec3i(0, 1, 0));

        if (blockOffset.z == Chunk::Size - 1)
            UpdateMeshInDirection(chunkPos, vec3i(0, 0, 1));
    }
}
