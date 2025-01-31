#pragma once

#include "World/BlockType.h"
#include "World/Blocks/AirBlock.h"
#include "World/Blocks/GrassBlock.h"
#include "World/Blocks/SolidBlock.h"
#include "World/Blocks/TallGrassBlock.h"
#include "World/Blocks/WoodBlock.h"

namespace Minecraft::Blocks
{
    inline vector<shared_ptr<BlockType>> BlockList = { };

    template<typename T, typename... Params>
    inline BlockType* CreateBlock(Params... params)
    {
        auto block = make_shared<T>(params...);
        BlockList.push_back(block);
        return block.get();
    }

    inline BlockType* Air = CreateBlock<AirBlock>();
    inline BlockType* Stone = CreateBlock<SolidBlock>(vec2i(0, 1));
    inline BlockType* Dirt = CreateBlock<SolidBlock>(vec2i(1, 1));
    inline BlockType* Grass = CreateBlock<GrassBlock>(vec2i(1, 1));
    inline BlockType* TallGrass = CreateBlock<TallGrassBlock>(vec2i(4, 1));
    inline BlockType* Sand = CreateBlock<SolidBlock>(vec2i(5, 1));
    inline BlockType* Clay = CreateBlock<SolidBlock>(vec2i(6, 1));
    inline BlockType* IronOre = CreateBlock<SolidBlock>(vec2i(7, 1));
    inline BlockType* Wood = CreateBlock<WoodBlock>(vec2i(0, 2));
    inline BlockType* Leaves = CreateBlock<SolidBlock>(vec2i(2, 2));
}
