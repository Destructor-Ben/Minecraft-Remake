#include "Chunk.h"

namespace Minecraft
{
    Block Chunk::GetBlockInChunk(uint8_t BlockX, uint8_t BlockY, uint8_t BlockZ) {
        return blocks_[BlockX*ChunkSize*ChunkSize + BlockY*ChunkSize + BlockZ];
    };

    void Chunk::SetBlockInChunk(uint8_t BlockX, uint8_t BlockY, uint8_t BlockZ, Block Block) {
        blocks_[BlockX*ChunkSize*ChunkSize + BlockY*ChunkSize + BlockZ] = Block;
    };
}