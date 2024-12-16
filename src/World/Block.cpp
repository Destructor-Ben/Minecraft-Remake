#include "Block.h"

#include "World/Chunk.h"

namespace Minecraft
{
    Block::Block(Chunk& chunk, byte blockX, byte blockY, byte blockZ, BlockData& data) :
        m_Chunk(chunk),
        m_BlockX(blockX),
        m_BlockY(blockY),
        m_BlockZ(blockZ),
        Data(data) { }

    vec3 Block::GetWorldPos() const
    {
        return vec3(m_Chunk.GetWorldPos().x + m_BlockX, m_Chunk.GetWorldPos().y + m_BlockY, m_Chunk.GetWorldPos().z + m_BlockZ);
    }
}
