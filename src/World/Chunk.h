#pragma once

#include "../Common.h"
#include "Block.h"

uint8_t const ChunkX = 16;
uint8_t const ChunkY = 16;
uint8_t const ChunkZ = 16;

namespace Minecraft
{
    class Chunk
    {
        private:
            Block blocks_[ChunkX*ChunkY*ChunkZ];
        public:
            uint16_t GetChunk();
    };
}