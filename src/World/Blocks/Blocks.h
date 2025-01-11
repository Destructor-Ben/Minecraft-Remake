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
    inline unique_ptr<BlockType> Stone = make_unique<SolidBlock>(vec2i(0, 1));
    inline unique_ptr<BlockType> Dirt = make_unique<SolidBlock>(vec2i(1, 1));
    inline unique_ptr<BlockType> Grass = make_unique<GrassBlock>(vec2i(1, 1));
    inline unique_ptr<BlockType> TallGrass = make_unique<TallGrassBlock>(vec2i(4, 1));
    inline unique_ptr<BlockType> Wood = make_unique<WoodBlock>(vec2i(0, 2));
    inline unique_ptr<BlockType> Leaves = make_unique<SolidBlock>(vec2i(2, 2));
}
