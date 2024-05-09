#include "Block.h"

#include "Chunk.h"

namespace Minecraft
{
    Block::Block(Chunk& chunk, uint8 blockX, uint8 blockY, uint8 blockZ) : m_Chunk(chunk), m_BlockX(blockX), m_BlockY(blockY), m_BlockZ(blockZ) { }

    uint32 Block::GetID() const
    {
        return m_BlockX * Chunk::Size * Chunk::Size + m_BlockY * Chunk::Size + m_BlockZ;
    }

    BlockData& Block::GetData()
    {
        return m_Chunk.GetBlockData(*this);
    }

    vec3i Block::GetWorldPos() const
    {
        return {m_Chunk.GetWorldPos().x + m_BlockX, m_Chunk.GetWorldPos().y + m_BlockY, m_Chunk.GetWorldPos().z + m_BlockZ};
    }
}
