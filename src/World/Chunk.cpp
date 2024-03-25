#include "Chunk.h"

namespace Minecraft
{
    Chunk::Chunk()
    {
        m_Blocks = new Block[Size * Size * Size];
    }

    Chunk::~Chunk()
    {
        delete[] m_Blocks;
    }

    Block& Chunk::GetBlock(int32 x, int32 y, int32 z)
    {
        return m_Blocks[x * Size * Size + y * Size + z];
    }
}
