#pragma once

#include "Common.h"

#include "BlockData.h"

namespace Minecraft
{
    // Forward declare since we can't include Chunk.h since it includes this file
    class Chunk;

    // Blocks are essentially an index to actual data, similar to an ECS
    class Block
    {
    public:
        Block(Chunk& chunk, uint8 blockX, uint8 blockY, uint8 blockZ);

        // Index of the block used for accessing the arrays of data in the chunks
        uint32 GetID() const;
        Chunk& GetChunk() const { return m_Chunk; }
        BlockData& GetData();

        vec3i GetWorldPos() const;
        vec3i GetBlockPos() const { return {m_BlockX, m_BlockY, m_BlockZ}; }

    private:
        Chunk& m_Chunk;
        uint8 m_BlockX;
        uint8 m_BlockY;
        uint8 m_BlockZ;
    };
}
