#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    static constexpr float GrassChance = 0.15f;
    // TODO: probably use a voronoi texture to generate trees to avoid them being too close
    static constexpr float TreeChance = 0.005f;

    static constexpr int MinTreeHeight = 3;
    static constexpr int MaxTreeHeight = 5;

    static constexpr int MinJungleTreeHeight = 7;
    static constexpr int MaxJungleTreeHeight = 11;

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

    void WorldGenerator::GenerateTrees(Minecraft::Chunk& chunk)
    {
        // TODO: use for_block_in_chunk
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    /*/ Check if this block contains part of a tree
                    Block block = chunk.GetBlock(x, y, z);
                    auto blockType = GetTreeBlock(block);
                    if (blockType == nullopt)
                        continue;

                    // Place the block
                    block.Data.Type = blockType.value();
                    //*/
                }
            }
        }
    }

    // TODO: idk how to implement this
    optional<BlockType*> WorldGenerator::GetTreeBlock(Block& block)
    {
        for (int x = 0; x < 10; ++x)
        {
            for (int y = 0; y < 10; ++y)
            {
                for (int z = 0; z < 10; ++z)
                {
                    if (ContainsTreeOrigin(block))
                        return Blocks::Wood;
                }
            }
        }

        return nullopt;
    }

    bool WorldGenerator::ContainsTreeOrigin(Block& block)
    {
        vec3 pos = block.GetWorldPos();

        // Random check
        if (m_Noise.White3D(pos, TreeMap) > TreeChance)
            return false;

        // Ground check
        if (GetTerrainHeight(pos) + 1 != pos.y)
            return false;

        // Biome check
        Biome* biome = block.Data.Biome;
        if (biome != Biomes::Forest && biome != Biomes::Jungle && biome != Biomes::SnowyForest)
            return false;

        return true;
    }

    int WorldGenerator::GetTreeHeight(Block& block)
    {
        bool isJungle = block.Data.Biome == Biomes::Jungle;
        float minHeight = isJungle ? MinJungleTreeHeight : MinTreeHeight;
        float maxHeight = isJungle ? MaxJungleTreeHeight : MaxTreeHeight;

        return (int)std::lerp(minHeight, maxHeight, m_Noise.White3D(block.GetWorldPos(), TreeHeightMap));
    }
}
