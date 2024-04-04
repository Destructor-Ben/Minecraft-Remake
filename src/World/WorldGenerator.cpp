#include "WorldGenerator.h"

#include "World.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(class World& world, uint32 seed) : m_World(&world), m_Seed(seed) { }

    void WorldGenerator::Generate()
    {
        // TODO: once I have a proper chunking system, make this generate properly
        Generate(*m_World->Chunk);
        Generate(*m_World->Chunk2);
    }

    void WorldGenerator::Generate(int32 chunkX, int32 chunkY, int32 chunkZ)
    {
        // TODO: once I have a proper chunking system, make this generate properly
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

                    if (block.GetY() > -2)
                        continue;

                    type = BlockType::Dirt;
                }
            }
        }
    }
}
