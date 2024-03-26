#pragma once

#include "../Common.h"

#include <array>
#include <map>

// TODO: fix ciruclar dependency loop
#include "Block.h"
#include "Entity.h"

namespace Minecraft
{
    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    // TODO: entities
    class Chunk
    {
    public:
        static const uint8 Size = 10;

        const int32 X;
        const int32 Y;
        const int32 Z;

        // Block data TODO

        Chunk(int32 x, int32 y, int32 z): X(x), Y(y), Z(z) { }

        // TODO: use indexing operator for blocks
        Block GetBlock(uint8 localX, uint8 localY, uint8 localZ);

        template<typename T>
        T GetBlockData(Block block);

        // TODO: possibly make iterators for entities and blocks in a chunk

        // TODO: tick, update, and render chunks
    };
}
