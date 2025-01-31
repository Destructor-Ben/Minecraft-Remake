#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    static FractalNoiseParams HeightMap = { 9548762, 8, 0.05f };

    void WorldGenerator::GenerateTerrain(Chunk& chunk)
    {
        const int MinHeight = -20;
        const int MaxHeight = 20;

        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int z = 0; z < Chunk::Size; ++z)
            {
                // Generate height
                float xCoord = x + chunk.GetWorldPos().x;
                float zCoord = z + chunk.GetWorldPos().z;

                float noiseValue = m_Noise.Fractal2D(xCoord, zCoord, HeightMap);
                int height = std::lerp(MinHeight, MaxHeight, noiseValue);

                // Set blocks
                for (int y = 0; y < Chunk::Size; ++y)
                {
                    Block block = chunk.GetBlock(x, y, z);
                    float yPos = block.GetWorldPos().y;

                    // Set the block type
                    auto type = Blocks::Air;

                    if (yPos == height)
                    {
                        if (block.Data.Biome == Biomes::Grassland)
                            type = Blocks::Grass;
                        else if (block.Data.Biome == Biomes::Forest)
                            type = Blocks::Wood;
                        else if (block.Data.Biome == Biomes::Desert)
                            type = Blocks::Sand;
                        else if (block.Data.Biome == Biomes::Jungle)
                            type = Blocks::Leaves;
                        else if (block.Data.Biome == Biomes::Tundra)
                            type = Blocks::IronOre;
                        else if (block.Data.Biome == Biomes::SnowyForest)
                            type = Blocks::Clay;
                    }

                    if (yPos < height)
                        type = Blocks::Dirt;

                    if (yPos <= height - 2)
                        type = Blocks::Stone;

                    block.Data.Type = type;
                }
            }
        }
    }
}
