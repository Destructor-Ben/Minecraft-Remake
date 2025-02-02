#pragma once

#include "World/Block.h"
#include "World/BlockData.h"

namespace Minecraft
{
    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    class Chunk
    {
    public:
        static constexpr byte Size = 10;
        static constexpr uint Volume = Size * Size * Size;

        // Parameterless ctor only needed for use in unordered_map
        Chunk() = default;
        Chunk(int chunkX, int chunkY, int chunkZ) : m_ChunkX(chunkX), m_ChunkY(chunkY), m_ChunkZ(chunkZ) { }

        bool ContainsBlockPos(int blockX, int blockY, int blockZ);
        bool ContainsBlockPos(vec3i blockPos) { return ContainsBlockPos(blockPos.x, blockPos.y, blockPos.z); }

        Block GetBlock(byte blockX, byte blockY, byte blockZ);
        Block GetBlock(vec3i blockPos) { return GetBlock((byte)blockPos.x, (byte)blockPos.y, (byte)blockPos.z); }

        // Index of the block used for accessing the arrays of data in the chunks
        static uint GetBlockID(Block block) { return GetBlockID(block.GetBlockPos()); }
        static uint GetBlockID(vec3i blockPos) { return GetBlockID((byte)blockPos.x, (byte)blockPos.y, (byte)blockPos.z); }
        static uint GetBlockID(byte blockX, byte blockY, byte blockZ);

        BlockData& GetBlockData(uint blockID);

        vec3 GetWorldPos() const { return vec3(GetChunkPos() * (int)Size); }
        vec3i GetChunkPos() const { return { m_ChunkX, m_ChunkY, m_ChunkZ }; }

        void RegenerateMesh();

        void Tick();
        void Update();
        void Render();

    private:
        int m_ChunkX = 0;
        int m_ChunkY = 0;
        int m_ChunkZ = 0;

        array <BlockData, Volume> m_BlockData = { };
    };
}
