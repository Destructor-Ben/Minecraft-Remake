#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    static constexpr int MinHeight = -50;
    static constexpr int MaxHeight = 50;

    static constexpr int MinDirtAmount = 2;
    static constexpr int MaxDirtAmount = 3;

    static FractalNoiseParams HeightMap = { 9548762, 8, 0.005f };
    static WhiteNoiseParams DirtDepthMap = { 9328546 };

    void WorldGenerator::GenerateTerrain(Chunk& chunk)
    {
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int z = 0; z < Chunk::Size; ++z)
            {
                // Get height
                vec3 samplePos = { x + chunk.GetWorldPos().x, 0, z + chunk.GetWorldPos().z };
                int height = GetTerrainHeight(samplePos);

                // Set blocks
                for (int y = 0; y < Chunk::Size; ++y)
                {
                    Block block = chunk.GetBlock(x, y, z);
                    float yPos = block.GetWorldPos().y;

                    // Set the block type
                    auto biome = block.Data.Biome;
                    auto blockType = Blocks::Air;
                    int surfaceHeight = GetSurfaceHeight(samplePos);

                    // Surface
                    if (yPos == height)
                        blockType = m_SurfaceBlocksMap[biome];

                    // Underground
                    if (yPos < height)
                        blockType = m_UndergroundBlocksMap[biome];

                    // Rock
                    if (yPos <= height - surfaceHeight)
                        blockType = Blocks::Stone;

                    block.Data.Type = blockType;
                }
            }
        }
    }

    int WorldGenerator::GetTerrainHeight(vec3 pos)
    {
        float noiseValue = m_Noise.Fractal2D(pos.x, pos.z, HeightMap);
        return (int)std::lerp((float)MinHeight, (float)MaxHeight, noiseValue);
    }

    int WorldGenerator::GetSurfaceHeight(vec3 pos)
    {
        float noiseValue = m_Noise.White2D(pos.x, pos.z, DirtDepthMap);
        return (int)std::lerp((float)MinDirtAmount, (float)MaxDirtAmount, noiseValue);
    }

    void WorldGenerator::InitSurfaceBlocksMap()
    {
        m_SurfaceBlocksMap[Biomes::None] = Blocks::Air;
        m_UndergroundBlocksMap[Biomes::None] = Blocks::Air;

        m_SurfaceBlocksMap[Biomes::Grassland] = Blocks::Grass;
        m_UndergroundBlocksMap[Biomes::Grassland] = Blocks::Dirt;
        m_SurfaceBlocksMap[Biomes::Forest] = Blocks::Grass;
        m_UndergroundBlocksMap[Biomes::Forest] = Blocks::Dirt;

        m_SurfaceBlocksMap[Biomes::Desert] = Blocks::Sand;
        m_UndergroundBlocksMap[Biomes::Desert] = Blocks::Sand;
        m_SurfaceBlocksMap[Biomes::Jungle] = Blocks::Grass;
        m_UndergroundBlocksMap[Biomes::Jungle] = Blocks::Dirt;

        // TODO: proper blocks for snowy biomes
        m_SurfaceBlocksMap[Biomes::Tundra] = Blocks::IronOre;
        m_UndergroundBlocksMap[Biomes::Tundra] = Blocks::IronOre;
        m_SurfaceBlocksMap[Biomes::SnowyForest] = Blocks::Clay;
        m_UndergroundBlocksMap[Biomes::SnowyForest] = Blocks::Clay;
    }
}
