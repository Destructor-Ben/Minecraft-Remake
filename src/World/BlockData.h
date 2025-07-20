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
        // TODO: split into type + misc instance data, and type can be compressed with run length encoding/palette
        // - Data will cover everything else, and ill probably compress the entire chunk/region file anyway so no need to worry about extra data
        // -  It kind of already is split
        // - Also use y as the fastest changing aces for RLE since most bolcks will be the same on the y axis (the world is basically columns right?)
        BlockType* Type = Blocks::Air;
        BiomeType* Biome = Biomes::None;
    };
}
