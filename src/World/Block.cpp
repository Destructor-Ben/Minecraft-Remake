#include "Block.h"

#include "World/Chunk.h"

namespace Minecraft
{
    BlockPos Block::GetBlockPos() const
    {
        return m_BlockOffset.ToBlockPos(m_Chunk->GetChunkPos());
    }
}
