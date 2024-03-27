#pragma once

#include "../Common.h"

#include <array>
#include <map>

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

        Chunk(int32 x, int32 y, int32 z);
        ~Chunk();

        Block GetBlock(uint8 localX, uint8 localY, uint8 localZ);

        template<typename T>
        T& GetBlockData(Block block);

        template<typename T>
        void RemoveBlockData(Block block);

        // TODO: possibly make iterators for entities and blocks in a chunk

        // TODO: tick, update, and render chunks

    private:
        // TODO: what kind of collection should be used instead of a map?
        std::vector<std::map<uint16, void*>> m_BlockData;
    };
}

#endif
