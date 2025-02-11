#pragma once

#include "World/BlockType.h"
#include "World/Blocks/Blocks.h"
#include "World/BiomeType.h"
#include "World/Biomes/Biomes.h"

namespace Minecraft
{
    // Contains the instance block data
    class BlockData
    {
    public:
        BlockType* Type = Blocks::Air;
        BiomeType* Biome = Biomes::None;
    };
}
