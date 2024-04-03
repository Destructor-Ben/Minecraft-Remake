#include "Chunk.h"

namespace Minecraft
{
    Chunk::Chunk(int32 x, int32 y, int32 z): X(x), Y(y), Z(z)
    {

    }

    Block Chunk::GetBlock(uint8 localX, uint8 localY, uint8 localZ)
    {
        return Block(*this, localX, localY, localZ);
    }

    template<typename T>
    T& Chunk::GetBlockData(Block block)
    {
        return m_BlockData[T::GetID()][block.GetID()];
    }

    template<typename T>
    T& Chunk::AddBlockData(Block block)
    {
        //m_BlockData[block.GetID()] = T();
        return GetBlockData<T>(block);
    }

    template<typename T>
    void Chunk::RemoveBlockData(Block block)
    {
        //m_BlockData[T::GetID()].erase(block.GetID());
    }
}
