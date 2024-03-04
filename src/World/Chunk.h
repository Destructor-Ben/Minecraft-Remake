#pragma once

#include "../Common.h"
#include "Block.h"

uint8_t const ChunkSize = 16;

namespace Minecraft
{
    class Chunk
    {
        private:
            Block blocks_[ChunkSize*ChunkSize*ChunkSize];
        public:
            Block GetBlockInChunk(uint8_t BlockX, uint8_t BlockY, uint8_t BlockZ);
            void SetBlockInChunk(uint8_t BlockX, uint8_t BlockY, uint8_t BlockZ, Block Block);
    };
}