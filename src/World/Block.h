#pragma once

#include "World/Coords.h"

namespace Minecraft
{
    class BlockData;
    class Chunk;

    // Blocks are essentially an index to actual data, similar to an ECS
    class Block
    {
    public:
        BlockData& Data; // Set when the block is created

        Block(Chunk& chunk, const BlockOffset& blockOffset, BlockData& data);

        Chunk& GetChunk() const { return m_Chunk; }
        BlockPos GetBlockPos() const;
        BlockOffset GetBlockOffset() const { return m_BlockOffset; }

    private:
        Chunk& m_Chunk;
        BlockOffset m_BlockOffset;
    };
}
