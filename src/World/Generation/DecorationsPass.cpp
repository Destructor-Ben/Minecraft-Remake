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
        // Trees then grass so trees don't get eaten into by grass
        GenerateTrees(chunk);
        GenerateGrass(chunk);
    }

    void WorldGenerator::GenerateGrass(Chunk& chunk)
    {
        // TODO: use for_block_in_chunk
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    // Check if grass can be placed
                    Block block = chunk.GetBlock(BlockOffset(x, y, z));
                    if (!ContainsGrass(block))
                        continue;

                    // Place the grass
                    block.Data->Type = Blocks::TallGrass;
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

        // Air check
        if (block.Data->Type != Blocks::Air)
            return false;

        // Biome check
        BiomeType* biome = block.Data->Biome;
        if (biome == Biomes::Tundra || biome == Biomes::SnowyForest || biome == Biomes::Desert)
            return false;

        return true;
    }

    // TODO: abstract multi block structures away at some point because this is a lot of boilerplate
    // TODO: this code is ugly
    void WorldGenerator::GenerateTrees(Minecraft::Chunk& chunk)
    {
        // TODO: use for_block_in_chunk

        // This contains all tree positions in the current chunk
        vector <vec3i> treePositions = { };

        // Add tree positions from this chunk and nearby chunks
        // TODO: make sure these specific values work
        constexpr int TreeBoundsWidth = 5;
        constexpr int TreeBoundsHeight = MaxJungleTreeHeight + 2;

        for (int x = -TreeBoundsWidth; x < Chunk::Size + TreeBoundsWidth; ++x)
        {
            for (int y = -TreeBoundsHeight; y < Chunk::Size + TreeBoundsHeight; ++y)
            {
                for (int z = -TreeBoundsWidth; z < Chunk::Size + TreeBoundsWidth; ++z)
                {
                    vec3i pos = vec3(x, y, z);
                    if (ContainsTreeOrigin(pos + (vec3i)chunk.GetWorldPos()))
                        treePositions.push_back(pos);
                }
            }
        }

        // Actually place the trees
        for (auto& treeOrigin : treePositions)
        {
            PlaceTree(chunk, treeOrigin + (vec3i)chunk.GetWorldPos(), treeOrigin);
        }
    }

    // This doesn't go across chunk borders, so other chunks will generate trees differently when they are done
    void WorldGenerator::PlaceTree(Chunk& chunk, vec3i pos, vec3i blockPos)
    {
        int treeHeight = GetTreeHeight(pos);
        vec3i topOfTree = blockPos;
        topOfTree.y += treeHeight - 1;

        // Trunk
        for (int trunkY = 0; trunkY < treeHeight; ++trunkY)
        {
            auto trunkPos = BlockOffset(blockPos);
            trunkPos.y += trunkY;

            if (!chunk.ContainsPos(trunkPos))
                continue;

            auto block = chunk.GetBlock(trunkPos);
            block.Data->Type = Blocks::Wood;
        }

        // Leaves
        for (int x = 0; x < 5; ++x)
        {
            for (int y = 0; y < 2; ++y)
            {
                for (int z = 0; z < 5; ++z)
                {
                    auto leafPos = BlockOffset(topOfTree);
                    leafPos.Pos += vec3i(x - 2, y - 1, z - 2);

                    if (!chunk.ContainsPos(leafPos))
                        continue;

                    auto block = chunk.GetBlock(leafPos);

                    if (block.Data->Type != Blocks::Air)
                        continue;

                    block.Data->Type = Blocks::Leaves;
                }
            }
        }

        for (int x = 0; x < 3; ++x)
        {
            for (int y = 0; y < 2; ++y)
            {
                for (int z = 0; z < 3; ++z)
                {
                    auto leafPos = BlockOffset(topOfTree);
                    leafPos.Pos += vec3i(x - 1, y + 1, z - 1);

                    if (!chunk.ContainsPos(leafPos))
                        continue;

                    auto block = chunk.GetBlock(leafPos);

                    if (block.Data->Type != Blocks::Air)
                        continue;

                    block.Data->Type = Blocks::Leaves;
                }
            }
        }
    }

    bool WorldGenerator::ContainsTreeOrigin(vec3i pos)
    {
        // Random check
        if (m_Noise.White3D(pos, TreeMap) > TreeChance)
            return false;

        // Ground check
        if (GetTerrainHeight(pos) + 1 != pos.y)
            return false;

        // Biome check
        BiomeType* biome = CalculateBiome(pos);
        if (biome != Biomes::Forest && biome != Biomes::Jungle && biome != Biomes::SnowyForest)
            return false;

        return true;
    }

    int WorldGenerator::GetTreeHeight(vec3i pos)
    {
        bool isJungle = CalculateBiome(pos) == Biomes::Jungle;
        float minHeight = isJungle ? MinJungleTreeHeight : MinTreeHeight;
        float maxHeight = isJungle ? MaxJungleTreeHeight : MaxTreeHeight;

        return (int)std::lerp(minHeight, maxHeight, m_Noise.White3D(pos, TreeHeightMap));
    }
}
