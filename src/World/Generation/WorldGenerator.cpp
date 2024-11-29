#include "WorldGenerator.h"

#include "Random/Perlin.h"
#include "World/World.h"

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

        // Create meshes
        for (auto& chunk : m_World->Chunks | views::values)
        {
            chunk.RegenerateMesh();
        }
    }

    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos)
    {
        // TODO: check the radius of the chunks around the player, and if they don't exit, generate them
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
                    BlockType& type = block.GetData().Type;
                    type = BlockType::Air;

                    if (block.GetWorldPos().y <= height)
                        type = BlockType::Dirt;

                    if (block.GetWorldPos().y <= height - 2)
                        type = BlockType::Stone;
                }
            }
        }
    }

    double WorldGenerator::GenerateHeight(float verticalScale, float horizontalScale, uint seed, int x, int z, double persistence, double octaveCount)
    {
        return Perlin2D(seed, x * horizontalScale, z * horizontalScale, persistence, octaveCount) * verticalScale;
    }
}
