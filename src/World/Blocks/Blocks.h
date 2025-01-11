#pragma once

#include "World/BlockType.h"
#include "World/Blocks/AirBlock.h"
#include "World/Blocks/GrassBlock.h"
#include "World/Blocks/SolidBlock.h"
#include "World/Blocks/TallGrassBlock.h"
#include "World/Blocks/WoodBlock.h"

namespace Minecraft::Blocks
{
    inline unique_ptr<BlockType> Air = make_unique<AirBlock>();
    inline unique_ptr<BlockType> Stone = make_unique<SolidBlock>(2);
    inline unique_ptr<BlockType> Dirt = make_unique<SolidBlock>(3);
    inline unique_ptr<BlockType> Grass = make_unique<GrassBlock>(3);
    inline unique_ptr<BlockType> TallGrass = make_unique<TallGrassBlock>(6);
    inline unique_ptr<BlockType> Wood = make_unique<WoodBlock>(7);
    inline unique_ptr<BlockType> Leaves = make_unique<SolidBlock>(9);
}
