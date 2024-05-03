#include "WorldGenerator.h"

#include "World.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(class World* world, uint32 seed) : m_World(world), m_Seed(seed)
    {
    }

    void WorldGenerator::Generate()
    {
        // TODO: once I have a proper chunking system, make this generate properly
        for (auto& chunk : m_World->Chunks)
        {
            Generate(chunk);
        }
    }

    void WorldGenerator::Generate(int32 chunkX, int32 chunkY, int32 chunkZ)
    {
        // TODO: once I have a proper chunking system, make this generate properly
    }

    // TODO: make Perlin function and make this use it
    int16 WorldGenerator::GenerateHeightAtBlock(float VerticalScale, float HorizontalScale, uint32 seed, Block& block, double Persistence, double OctaveCount){
        return Perlin2D(seed, block.GetX(), block.GetZ() * HorizontalScale, Persistence, OctaveCount) * VerticalScale + block.GetY() + 3 > 0;
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

                    const float VerticalScale = 4.0f;
                    const float HorizontalScale = 0.5f;

                    if (GenerateHeightAtBlock(VerticalScale, HorizontalScale, 0, block, 0.5, 4) > 0)  // TODO: pass seed to function
                        continue;

                    type = BlockType::Dirt;
                }
            }
        }
    }
}
