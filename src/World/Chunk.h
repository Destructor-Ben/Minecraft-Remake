#pragma once

#include "World/Coords.h"
#include "World/Block.h"
#include "World/BlockData.h"

namespace Minecraft
{
    class BoundingBox;

    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    // TODO: add some fields that are recalculated when the chunk is loaded/modified
    // - IsEmpty - whether the entire chunk is air
    // - BitField for blocks that are exposed to air
    //   - Used to significantly speed up chunk meshing
    //   - Comments are also in ChunkRendererer
    // - Maybe a BitField for solid blocks too, could be useful for collision
    // - 64 uint64s (ulongs) are needed for a bitfield
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

        Block GetBlock(byte x, byte y, byte z) { return GetBlock(BlockOffset(x, y, z)); }
        Block GetBlock(const BlockOffset& blockOffset);
        BlockData* GetBlockData(uint blockID);

        bool ContainsPos(const vec3& worldPos) const;
        bool ContainsPos(const BlockPos& blockPos) const;
        bool ContainsPos(const BlockOffset& offset) const;
        BoundingBox GetBounds() const;

        vec3 GetWorldPos() const { return m_ChunkPos.ToWorldPos(); }
        ChunkPos GetChunkPos() const { return m_ChunkPos; }

    private:
        ChunkPos m_ChunkPos = { };
        array <BlockData, Volume> m_BlockData = { };
    };
}
