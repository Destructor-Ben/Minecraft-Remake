#include "WorldGenerator.h"

#include "Game.h"
#include "LogManager.h"
#include "Random/Perlin.h"
#include "World/World.h"
#include "World/Blocks/Blocks.h"

namespace Minecraft
{
    // TODO: make min and max world height (in chunks) be stored in World.h
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

        // TODO: verify that this isn't being weird with chunk radii
        for (int x = -GenerationRadius + 1; x < GenerationRadius; ++x)
        {
            for (int z = -GenerationRadius + 1; z < GenerationRadius; ++z)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    // Calculate chunk pos
                    auto chunkPos = vec3i(x, y, z);
                    // TODO: in the future, just chunkPos += playerChunkPos, current impl makes it easier to test because no y axis
                    chunkPos.x += playerChunkPos.x;
                    chunkPos.z += playerChunkPos.z;

                    // Check if the chunk exists
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

    double WorldGenerator::GenerateHeight(float verticalScale, float horizontalScale, uint seed, int x, int z, double persistence, double octaveCount)
    {
        return sin(x * 3.14159265359 * 0.5) * sin(z * 1.5) * 10.0;
        // return Perlin2D(seed, x * horizontalScale, z * horizontalScale, persistence, octaveCount) * verticalScale;
    }
}
