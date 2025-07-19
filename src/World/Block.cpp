#include "Block.h"

#include "World/Chunk.h"

namespace Minecraft
{
    Block::Block(Chunk& chunk, const BlockOffset& blockOffset, BlockData& data) :
        Data(data),
        m_Chunk(chunk),
        m_BlockOffset(blockOffset) { }

    BlockPos Block::GetBlockPos() const
    {
        return m_BlockOffset.ToBlockPos(m_Chunk.GetChunkPos());
    }
}
