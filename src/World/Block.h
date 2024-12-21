#pragma once

namespace Minecraft
{
    class BlockData;
    class Chunk;

    // Blocks are essentially an index to actual data, similar to an ECS
    class Block
    {
    public:
        BlockData& Data; // Set when the block is created

        Block(Chunk& chunk, byte blockX, byte blockY, byte blockZ, BlockData& data);

        Chunk& GetChunk() const { return m_Chunk; }
        vec3 GetWorldPos() const;
        vec3i GetBlockPos() const { return { m_BlockX, m_BlockY, m_BlockZ }; }

    private:
        Chunk& m_Chunk;
        byte m_BlockX;
        byte m_BlockY;
        byte m_BlockZ;
    };
}
