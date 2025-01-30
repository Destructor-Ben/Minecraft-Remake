#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    void WorldGenerator::GenerateDecorations(Chunk& chunk)
    {
        // TODO: redo
        /*/
        // Generate trees and grass
        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                for (int y = 0; y < Chunk::Size; y++)
                {
                    // Find a dirt or grass block with air above it
                    Block block = chunk.GetBlock(x, y, z);

                    auto blockType = block.Data.Type;
                    if (blockType != Blocks::Dirt.get() && blockType != Blocks::Grass.get())
                        continue;

                    int yAbove = y + 1;
                    // TODO: fix across chunk borders
                    if (yAbove >= Chunk::Size)
                        continue;

                    Block blockAbove = chunk.GetBlock(x, yAbove, z);
                    if (blockAbove.Data.Type != Blocks::Air.get())
                        continue;

                    // Place the thing
                    // TODO: proper random chances
                    const float Chance = 0.1f;
                    if (true)//m_Noise.White3D(x, yAbove, z) > Chance)
                        continue;

                    blockAbove.Data.Type = Blocks::TallGrass.get();
                }
            }
        }

        // TODO: avoid copy paste
        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                for (int y = 0; y < Chunk::Size; y++)
                {
                    // Find a dirt or grass block with air above it
                    Block block = chunk.GetBlock(x, y, z);

                    // Biome check
                    if (block.Data.Biome != Biomes::Forest.get())
                        continue;

                    auto blockType = block.Data.Type;
                    if (blockType != Blocks::Dirt.get() && blockType != Blocks::Grass.get())
                        continue;

                    int yAbove = y + 1;
                    // TODO: fix across chunk borders
                    if (yAbove >= Chunk::Size)
                        continue;

                    Block blockAbove = chunk.GetBlock(x, yAbove, z);
                    if (blockAbove.Data.Type != Blocks::Air.get())
                        continue;

                    // Place the thing
                    // TODO: proper random chances - Since this uses the same noise map as the grass, it will fail to place when the chances are the same
                    const float Chance = 0.01f;
                    if (true)//m_Noise.White3D(x + 1024, yAbove + 1024, z + 1024) > Chance)
                        continue;

                    int height = round(glm::lerp(5.0f, 7.0f, 0.0f));//m_Noise.White3D(x + 2048, y + 2048, z + 2048)));

                    // Trunk
                    for (int yTrunk = 1; yTrunk <= height; ++yTrunk)
                    {
                        // TODO: fix across chunk borders
                        if (y + yTrunk >= Chunk::Size)
                            continue;

                        Block trunkBlock = chunk.GetBlock(x, y + yTrunk, z);
                        trunkBlock.Data.Type = Blocks::Wood.get();
                    }

                    // Leaves
                    for (int leafX = -1; leafX <= 1; ++leafX)
                    {
                        for (int leafZ = -1; leafZ <= 1; ++leafZ)
                        {
                            for (int leafY = -1; leafY <= 1; ++leafY)
                            {
                                int leafFinalX = x + leafX;
                                int leafFinalY = y + height + leafY;
                                int leafFinalZ = z + leafZ;

                                // TODO: fix across chunk borders
                                if (leafFinalX >= Chunk::Size || leafFinalX < 0)
                                    continue;

                                if (leafFinalY >= Chunk::Size || leafFinalY < 0)
                                    continue;

                                if (leafFinalZ >= Chunk::Size || leafFinalZ < 0)
                                    continue;

                                Block leafBlock = chunk.GetBlock(leafFinalX, leafFinalY, leafFinalZ);

                                if (leafBlock.Data.Type == Blocks::Air.get())
                                    leafBlock.Data.Type = Blocks::Leaves.get();
                            }
                        }
                    }

                    for (int leafX = -2; leafX <= 2; ++leafX)
                    {
                        for (int leafZ = -2; leafZ <= 2; ++leafZ)
                        {
                            for (int leafY = -2; leafY <= -1; ++leafY)
                            {
                                int leafFinalX = x + leafX;
                                int leafFinalY = y + height + leafY;
                                int leafFinalZ = z + leafZ;

                                // TODO: fix across chunk borders
                                if (leafFinalX >= Chunk::Size || leafFinalX < 0)
                                    continue;

                                if (leafFinalY >= Chunk::Size || leafFinalY < 0)
                                    continue;

                                if (leafFinalZ >= Chunk::Size || leafFinalZ < 0)
                                    continue;

                                Block leafBlock = chunk.GetBlock(leafFinalX, leafFinalY, leafFinalZ);

                                if (leafBlock.Data.Type == Blocks::Air.get())
                                    leafBlock.Data.Type = Blocks::Leaves.get();
                            }
                        }
                    }
                }
            }
        }
         //*/
    }
}
