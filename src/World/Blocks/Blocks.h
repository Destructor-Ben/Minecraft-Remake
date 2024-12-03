#pragma once

#include "World/BlockType.h"
#include "World/Blocks/AirBlock.h"
#include "World/Blocks/GrassBlock.h"
#include "World/Blocks/SolidBlock.h"

namespace Minecraft::Blocks
{
    inline BlockType Air = AirBlock();
    inline BlockType Stone = SolidBlock(2);
    inline BlockType Dirt = SolidBlock(3);
    inline BlockType Grass = GrassBlock(3);
}
