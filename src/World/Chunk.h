#pragma once

#include "../Common.h"

#include "Block.h"
#include "BlockData.h"
#include "BlockType.h"
#include "Entity.h"

namespace Minecraft
{
    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    class Chunk
    {
    public:
        static const uint8 Size = 10;

        const int32 X;
        const int32 Y;
        const int32 Z;

        Chunk(int32 x, int32 y, int32 z);

        Block GetBlock(uint8 localX, uint8 localY, uint8 localZ);

        BlockData& GetBlockData(Block block) { return m_BlockData[block.GetID()]; }

        // TODO: make block data components
        /*/
        template<typename T>
        T& GetBlockData(Block block);

        template<typename T>
        T& AddBlockData(Block block);

        template<typename T>
        void RemoveBlockData(Block block);
        //*/

        // TODO: possibly make iterators for entities and blocks in a chunk

        // TODO: entities

        void Tick();
        void Update();
        void Render();

    private:
        std::array<BlockData, Size * Size * Size> m_BlockData = std::array<BlockData, Size * Size * Size>();

        //std::vector<std::array<BlockType, Size * Size * Size>> m_BlockData = std::vector<std::array<BlockType, Size * Size * Size>>();
    };
}
