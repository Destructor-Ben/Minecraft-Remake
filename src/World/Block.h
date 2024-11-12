#pragma once

namespace Minecraft
{
    class BlockData;
    class Chunk;

    // Blocks are essentially an index to actual data, similar to an ECS
    class Block
    {
    public:
        Block(Chunk& chunk, byte blockX, byte blockY, byte blockZ);

        // Index of the block used for accessing the arrays of data in the chunks
        uint GetID() const;
        Chunk& GetChunk() const { return m_Chunk; }
        BlockData& GetData();

        vec3i GetWorldPos() const;
        vec3i GetBlockPos() const { return { m_BlockX, m_BlockY, m_BlockZ }; }

    private:
        Chunk& m_Chunk;
        byte m_BlockX;
        byte m_BlockY;
        byte m_BlockZ;
    };
}
