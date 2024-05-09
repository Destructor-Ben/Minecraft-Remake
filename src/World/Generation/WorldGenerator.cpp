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
        const int SpawnRadius = 8;
        for (int x = 0; x < SpawnRadius * 2; ++x)
        {
            for (int z = 0; z < SpawnRadius * 2; ++z)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    Chunk chunk(x - SpawnRadius, y, z - SpawnRadius);
                    m_World->Chunks.push_back(chunk);
                }
            }
        }

        // Generating world
        for (auto& chunk : m_World->Chunks)
        {
            Generate(chunk);
        }

        // Create meshes
        for (auto& chunk : m_World->Chunks)
        {
            // TODO: make a function for chunks to do this
            Renderer->ChunkRenderer->RegenerateMesh(chunk);
        }
    }

    // TODO: make Perlin function and make this use it
    int16 WorldGenerator::GenerateHeightAtBlock(float VerticalScale, float HorizontalScale, uint32 seed, Block& block, double Persistence, double OctaveCount)
    {
        return Perlin2D(seed, block.GetX() * HorizontalScale, block.GetZ() * HorizontalScale, Persistence, OctaveCount) * VerticalScale + block.GetY() + 3 > 0;
    }

    void WorldGenerator::Generate(Chunk& chunk)
    {
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
}
