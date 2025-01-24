#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    void WorldGenerator::GenerateTerrain(Chunk& chunk)
    {
        const int NoiseScale = 25.0f;
        const int Height = 25.0f;

        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                // Generate height
                float xCoord = x + chunk.GetWorldPos().x;
                float zCoord = z + chunk.GetWorldPos().z;

                xCoord /= NoiseScale;
                zCoord /= NoiseScale;

                float noiseValue = m_Noise.Fractal2D(xCoord, zCoord);
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
