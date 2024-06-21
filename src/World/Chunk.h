#pragma once

#include "Common.h"

#include "Block.h"
#include "BlockData.h"
#include "BlockType.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/Quad.h"

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
        static const uint8 Size = 10;
        static const uint32 Volume = Size * Size * Size;

        // Parameterless ctor only needed for use in unordered_map
        Chunk() = default;
        Chunk(int32 chunkX, int32 chunkY, int32 chunkZ) : m_ChunkX(chunkX), m_ChunkY(chunkY), m_ChunkZ(chunkZ) { }

        Block GetBlock(uint8 blockX, uint8 blockY, uint8 blockZ);
        Block GetBlock(vec3i blockPos) { return GetBlock((uint8)blockPos.x, (uint8)blockPos.y, (uint8)blockPos.z); }
        BlockData& GetBlockData(Block block) { return m_BlockData[block.GetID()]; }

        vec3i GetWorldPos() const { return GetChunkPos() * (int32)Size; }
        vec3i GetChunkPos() const { return { m_ChunkX, m_ChunkY, m_ChunkZ }; }

        void RegenerateMesh();

        void Tick();
        void Update();
        void Render();

    private:
        int32 m_ChunkX = 0;
        int32 m_ChunkY = 0;
        int32 m_ChunkZ = 0;

        array<BlockData, Volume> m_BlockData = { };
    };
}
