#include "WorldGenerator.h"

#include "Game.h"
#include "Profiler.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "World/World.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(World* world, ulong seed) :
        m_World(world),
        m_Seed(seed),
        m_Noise(seed)
    {
        // TODO: should this be moved to below? any long init things should happen on the worldgen/tick thread in future
        InitBiomeMap();
        InitSurfaceBlocksMap();
    }

    void WorldGenerator::Generate(int spawnRadius, int minHeight, int maxHeight)
    {
        Instance->PerfProfiler->Push("WorldGenerator::Generate");

        for_chunk_in_radius_2D(x, z, spawnRadius, {
            for (int y = minHeight; y <= maxHeight; ++y)
            {
                // Create and generate the chunk
                auto chunkPos = vec3i(x, y, z);
                auto& chunk = CreateChunk(chunkPos);

                // Remesh the chunk
                // TODO: priority
                Instance->ChunkGraphics->QueueMeshRegen(chunk);
            }
        })

        Instance->PerfProfiler->Pop();
    }

    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight)
    {
        Instance->PerfProfiler->Push("WorldGenerator::GenerateChunksAroundPlayer");

        // Only generate new chunks when moving along chunk borders
        if (!m_World->HasPlayerMovedChunks)
        {
            Instance->PerfProfiler->Pop();
            return;
        }

        auto playerChunkPos = WorldToChunkPos(playerPos);

        for_chunk_in_radius(x, y, z, radius, {
            // Calculate chunk pos
            auto chunkPos = vec3i(x, y, z);
            chunkPos += playerChunkPos;

            // Check if the y position is out of bounds
            if (chunkPos.y < minHeight || chunkPos.y > maxHeight)
                continue;

            // Check if the chunk exists
            auto existingChunk = m_World->GetChunk(chunkPos);
            if (existingChunk.has_value())
                continue;

            // Generate the chunk
            auto& chunk = CreateChunk(chunkPos);

            // Regenerate chunk meshes
            for (const auto& direction : m_ChunkRemeshDirections)
            {
                auto chunkToRemesh = m_World->GetChunk(chunkPos);
                if (chunkToRemesh.has_value())
                    Instance->ChunkGraphics->QueueMeshRegen(*chunkToRemesh.value()); // TODO: priority
            }
        })

        Instance->PerfProfiler->Pop();
    }

    Chunk& WorldGenerator::CreateChunk(vec3i chunkPos)
    {
        // TODO: this and below flood the console
        // TODO: make the profiler just add the duration onto calls of the same name + increment a counter so we know how many times a method was called
        //Instance->PerfProfiler->Push("WorldGenerator::CreateChunk");

        // Avoid copying the chunk into the map, just create it in there and retrieve its reference
        m_World->Chunks.emplace(chunkPos, Chunk(chunkPos.x, chunkPos.y, chunkPos.z));
        auto& chunk = m_World->Chunks[chunkPos];
        Generate(chunk);

        //Instance->PerfProfiler->Pop();

        return chunk;
    }

    // This is to stop a single block always getting a good random value
    // This would mean chests that spawn tend to be filled with good loot
    void WorldGenerator::Generate(Chunk& chunk)
    {
        //Instance->PerfProfiler->Push("WorldGenerator::GenerateChunk");

        GenerateBiomes(chunk);
        GenerateTerrain(chunk);
        GenerateCaves(chunk);
        GenerateStructures(chunk);
        GenerateDecorations(chunk);

        //Instance->PerfProfiler->Pop();
    }
}
