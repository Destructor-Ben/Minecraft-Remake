#include "WorldGenerator.h"

#include "Random/Perlin.h"
#include "World/World.h"
#include "World/Blocks/Blocks.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(World* world, uint seed) : m_World(world), m_Seed(seed)
    {
    }

    void WorldGenerator::Generate()
    {
        // Creating initial chunks
        // TODO: radii with chunks are weird, verify this is good
        const int SpawnRadius = 8;
        for (int x = 0; x < SpawnRadius * 2; ++x)
        {
            for (int z = 0; z < SpawnRadius * 2; ++z)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    Chunk chunk(x - SpawnRadius, y, z - SpawnRadius);
                    m_World->Chunks[vec3i(x - SpawnRadius, y, z - SpawnRadius)] = chunk;
                }
            }
        }

        // Generating world
        for (auto& chunk : m_World->Chunks | views::values)
        {
            Generate(chunk);
        }
    }

    // TODO: this doesn't work perfectly, only some directions generate chunks
    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos)
    {
        const int GenerationRadius = 3;
        auto playerChunkPos = WorldToChunkPos(playerPos);

        // When generating chunks side by side, many chunks will probably get re-meshed multiple times
        // So we queue them and use a set
        auto regenerateMeshQueue = set<Chunk*>();

        for (int x = playerChunkPos.x; x < GenerationRadius * 2; ++x)
        {
            for (int z = playerChunkPos.z; z < GenerationRadius * 2; ++z)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    // Check if the chunk exists
                    auto chunkPos = vec3i(x - GenerationRadius, y, z - GenerationRadius);
                    auto existingChunk = m_World->GetChunk(chunkPos);
                    if (existingChunk.has_value())
                        return;

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

    void WorldGenerator::Generate(Chunk& chunk)
    {
        // TODO: make generation passes
        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                // Generate height
                const float VerticalScale = 8.0f;
                const float HorizontalScale = 0.1f;
                int height = (int)GenerateHeight(VerticalScale, HorizontalScale, m_Seed, x + chunk.GetWorldPos().x, z + chunk.GetWorldPos().z, 0.5, 3);

                // Set blocks
                for (int y = 0; y < Chunk::Size; ++y)
                {
                    Block block = chunk.GetBlock(x, y, z);
                    float yPos = block.GetWorldPos().y;

                    // Set the block type
                    auto type = &Blocks::Air;

                    if (yPos == height)
                        type = &Blocks::Grass;

                    if (yPos < height)
                        type = &Blocks::Dirt;

                    if (yPos <= height - 2)
                        type = &Blocks::Stone;

                    block.Data.Type = type;
                }
            }
        }
    }

    double WorldGenerator::GenerateHeight(float verticalScale, float horizontalScale, uint seed, int x, int z, double persistence, double octaveCount)
    {
        return sin(x * 3.14159265359 * 0.5) * sin(z * 1.5) * 10.0;
        // return Perlin2D(seed, x * horizontalScale, z * horizontalScale, persistence, octaveCount) * verticalScale;
    }
}
