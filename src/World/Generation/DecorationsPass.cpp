#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    static constexpr float GrassChance = 0.15f;
    // TODO: probably use a voronoi texture to generate trees to avoid them being too close
    static constexpr float TreeChance = 0.01f;

    static WhiteNoiseParams GrassMap = { 12831823 };
    static WhiteNoiseParams TreeMap = { 332187563 };
    static WhiteNoiseParams TreeHeightMap = { 9982354 };

    void WorldGenerator::GenerateDecorations(Chunk& chunk)
    {
        GenerateGrass(chunk);
        GenerateTrees(chunk);
    }

    void WorldGenerator::GenerateGrass(Minecraft::Chunk& chunk)
    {
        // TODO: use for_block_in_chunk_2D
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    // Check if grass can be placed
                    vec3 samplePos = { x, y, z };
                    samplePos += chunk.GetWorldPos();

                    if (!ContainsGrass(samplePos))
                        continue;

                    // Place the grass
                    auto block = chunk.GetBlock(x, y, z);

                    // Biome check
                    // TODO: check the biome of the block below, and do it earlier - we want to avoid unnecessarily getting blocks though
                    if (block.Data.Biome == Biomes::Tundra || block.Data.Biome == Biomes::SnowyForest || block.Data.Biome == Biomes::Desert)
                        continue;

                    block.Data.Type = Blocks::TallGrass;
                }
            }
        }
    }

    void WorldGenerator::GenerateTrees(Minecraft::Chunk& chunk)
    {
        // TODO
    }

    bool WorldGenerator::ContainsGrass(vec3 pos)
    {
        // Random check
        if (m_Noise.White3D(pos, GrassMap) > GrassChance)
            return false;

        // Ground check
        int terrainHeight = GetTerrainHeight(pos);
        return terrainHeight + 1 == pos.y;
    }
}
