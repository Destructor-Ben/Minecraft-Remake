#include "WorldGenerator.h"

#include "World/World.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(World* world, uint seed) :
        m_World(world),
        m_Seed(seed),
        m_Noise(seed)
    {
        InitBiomeMap();
        InitSurfaceBlocksMap();
    }

    void WorldGenerator::Generate(int spawnRadius, int minHeight, int maxHeight)
    {
        for_chunk_in_radius_2D(x, z, spawnRadius, {
            for (int y = minHeight; y <= maxHeight; ++y)
            {
                auto chunkPos = vec3i(x, y, z);
                Chunk chunk(x, y, z);
                Generate(chunk);
                m_World->Chunks[chunkPos] = chunk;
            }
        })
    }

    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight)
    {
        auto playerChunkPos = WorldToChunkPos(playerPos);

        // When generating chunks side by side, many chunks will probably get re-meshed multiple times
        // So we queue them and use a set
        auto regenerateMeshQueue = set<Chunk*>();

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
            Chunk chunk(chunkPos.x, chunkPos.y, chunkPos.z);
            Generate(chunk); // Fucking value references get me sometimes, this needs to go BEFORE we set it, otherwise the empty chunk is copied to the Chunks map
            m_World->Chunks[chunkPos] = chunk;

            // Add chunks to re-mesh queue
            AddChunkIfExists(regenerateMeshQueue, chunkPos);

            AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(1, 0, 0));
            AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(-1, 0, 0));

            AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, 1, 0));
            AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, -1, 0));

            AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, 0, 1));
            AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, 0, -1));
        })

        // Regenerate chunk meshes
        for (auto chunk : regenerateMeshQueue)
        {
            chunk->RegenerateMesh();
        }
    }

    void WorldGenerator::AddChunkIfExists(set<Chunk*>& chunks, vec3i chunkPos)
    {
        auto chunk = m_World->GetChunk(chunkPos);
        if (chunk.has_value())
            chunks.insert(chunk.value());
    }

    // This is to stop a single block always getting a good random value
    // This would mean chests that spawn tend to be filled with good loot
    void WorldGenerator::Generate(Chunk& chunk)
    {
        GenerateBiomes(chunk);
        GenerateTerrain(chunk);
        GenerateCaves(chunk);
        GenerateStructures(chunk);
        GenerateDecorations(chunk);
    }
}
