#include "WorldGenerator.h"

#include "Game.h"
#include "LogManager.h"
#include "World/World.h"
#include "World/Blocks/Blocks.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(World* world, uint seed) :
        m_World(world),
        m_Seed(seed),
        m_Noise(seed) { }

    void WorldGenerator::Generate(int spawnRadius, int minHeight, int maxHeight)
    {
        for (int x = -spawnRadius + 1; x < spawnRadius; ++x)
        {
            for (int z = -spawnRadius + 1; z < spawnRadius; ++z)
            {
                for (int y = minHeight; y <= maxHeight; ++y)
                {
                    auto chunkPos = vec3i(x, y, z);
                    Chunk chunk(x, y, z);
                    Generate(chunk);
                    m_World->Chunks[chunkPos] = chunk;
                }
            }
        }
    }

    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight)
    {
        auto playerChunkPos = WorldToChunkPos(playerPos);

        // When generating chunks side by side, many chunks will probably get re-meshed multiple times
        // So we queue them and use a set
        auto regenerateMeshQueue = set<Chunk*>();

        for (int x = -radius + 1; x < radius; ++x)
        {
            for (int y = -radius + 1; y < radius; ++y)
            {
                for (int z = -radius + 1; z < radius; ++z)
                {
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
                }
            }
        }

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

    // TODO: fractal noise
    // TODO: make generation passes
    void WorldGenerator::Generate(Chunk& chunk)
    {
        const int NoiseScale = 25.0f;
        const int Height = 10.0f;

        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                // Generate height
                float xCoord = x + chunk.GetWorldPos().x;
                float zCoord = z + chunk.GetWorldPos().z;

                xCoord /= NoiseScale;
                zCoord /= NoiseScale;

                float noiseValue = m_Noise.Perlin2D(xCoord, zCoord);
                int height = (int)(noiseValue * Height);

                // Set blocks
                for (int y = 0; y < Chunk::Size; ++y)
                {
                    Block block = chunk.GetBlock(x, y, z);
                    float yPos = block.GetWorldPos().y;

                    // Set the block type
                    auto type = Blocks::Air.get();

                    if (yPos == height)
                        type = Blocks::Grass.get();

                    if (yPos < height)
                        type = Blocks::Dirt.get();

                    if (yPos <= height - 2)
                        type = Blocks::Stone.get();

                    block.Data.Type = type;
                }
            }
        }
    }
}
