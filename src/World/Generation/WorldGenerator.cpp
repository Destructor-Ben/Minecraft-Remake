#include "WorldGenerator.h"

#include "Game.h"
#include "World/World.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(class World* world, uint32 seed) : m_World(world), m_Seed(seed)
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
            for (int y = 0; y < Chunk::Size; y++)
            {
                for (int z = 0; z < Chunk::Size; z++)
                {
                    Block block = chunk.GetBlock(x, y, z);
                    BlockType& type = block.GetData().Type;
                    type = BlockType::Air;

                    const float VerticalScale = 8.0f;
                    const float HorizontalScale = 0.1f;

                    if (GenerateHeightAtBlock(VerticalScale, HorizontalScale, 0, block, 0.5, 3) > 0)  // TODO: pass seed to function
                        continue;

                    type = BlockType::Dirt;
                }
            }
        }
    }

    // TODO: make Perlin function and make this use it
    int16 WorldGenerator::GenerateHeightAtBlock(float VerticalScale, float HorizontalScale, uint32 seed, Block& block, double Persistence, double OctaveCount)
    {
        return Perlin2D(seed, block.GetWorldPos().x * HorizontalScale, block.GetWorldPos().z * HorizontalScale, Persistence, OctaveCount) * VerticalScale + block.GetWorldPos().y + 3 > 0;
    }
}
