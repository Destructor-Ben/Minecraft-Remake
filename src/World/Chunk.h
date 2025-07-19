#pragma once

#include "World/Coords.h"
#include "World/Block.h"
#include "World/BlockData.h"

namespace Minecraft
{
    class BoundingBox;

    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    class Chunk
    {
    public:
        static constexpr byte Size = 16;
        static constexpr uint Volume = Size * Size * Size;

        Chunk() = default; // Parameterless ctor only needed for use in unordered_map
        Chunk(const ChunkPos& pos) : m_ChunkPos(pos) { }

        // Index of the block used for accessing the arrays of data in the chunks
        static uint GetBlockID(const Block& block) { return GetBlockID(block.GetBlockOffset()); }
        static uint GetBlockID(const BlockOffset& blockOffset);

        Block GetBlock(const BlockOffset& blockOffset);
        BlockData& GetBlockData(uint blockID);

        bool ContainsPos(const vec3& pos) const;
        bool ContainsPos(const BlockPos& pos) const;
        bool ContainsPos(const BlockOffset& pos) const;
        BoundingBox GetBounds() const;

        vec3 GetWorldPos() const { return m_ChunkPos.ToWorldPos(); }
        ChunkPos GetChunkPos() const { return m_ChunkPos; }

    private:
        ChunkPos m_ChunkPos = { };
        array <BlockData, Volume> m_BlockData = { };
    };
}
