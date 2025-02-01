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
        // TODO: use for_block_in_chunk
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    // Check if grass can be placed
                    Block block = chunk.GetBlock(x, y, z);
                    if (!ContainsGrass(block))
                        continue;

                    // Place the grass
                    block.Data.Type = Blocks::TallGrass;
                }
            }
        }
    }

    void WorldGenerator::GenerateTrees(Minecraft::Chunk& chunk)
    {
        // TODO
    }

    bool WorldGenerator::ContainsGrass(Block& block)
    {
        vec3 pos = block.GetWorldPos();

        // Random check
        if (m_Noise.White3D(pos, GrassMap) > GrassChance)
            return false;

        // Ground check
        if (GetTerrainHeight(pos) + 1 != pos.y)
            return false;

        // Biome check
        Biome* biome = block.Data.Biome;
        if (biome == Biomes::Tundra || biome == Biomes::SnowyForest || biome == Biomes::Desert)
            return false;

        return true;
    }
}
