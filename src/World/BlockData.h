#pragma once

#include "World/BlockType.h"
#include "World/Blocks/Blocks.h"

namespace Minecraft
{
    // Contains the instance block data
    class BlockData
    {
    public:
        BlockType* Type = Blocks::Air.get();
    };
}
