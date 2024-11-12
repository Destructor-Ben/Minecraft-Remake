#pragma once

#include "World/Block.h"
#include "World/BlockData.h"

namespace Minecraft
{
    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    // TODO: make some form of static block data
    // TODO: make block data components
    // TODO: entities
    // TODO: getters that return a vector of all blocks/entities
    class Chunk
    {
    public:
        static const byte Size = 10;
        static const uint Volume = Size * Size * Size;

        // Parameterless ctor only needed for use in unordered_map
        Chunk() = default;
        Chunk(int chunkX, int chunkY, int chunkZ) : m_ChunkX(chunkX), m_ChunkY(chunkY), m_ChunkZ(chunkZ) { }

        Block GetBlock(byte blockX, byte blockY, byte blockZ);
        Block GetBlock(vec3i blockPos) { return GetBlock((byte)blockPos.x, (byte)blockPos.y, (byte)blockPos.z); }
        BlockData& GetBlockData(Block block) { return m_BlockData[block.GetID()]; }

        vec3i GetWorldPos() const { return GetChunkPos() * (int)Size; }
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
