#include "Chunk.h"

#include "BlockData.h"

namespace Minecraft
{
    Chunk::Chunk(int32 x, int32 y, int32 z): X(x), Y(y), Z(z)
    {
        // Push void* and then cast to whatever type we want (doesn't even need to be a pointer)
        // TODO: make a system for block data
        //m_BlockData.push_back(std::map<uint16, void*>());
    }

    Block Chunk::GetBlock(uint8 localX, uint8 localY, uint8 localZ)
    {
        return Block(*this, localX, localY, localZ);
    }

    template<typename T>
    T& Chunk::GetBlockData(Block block)
    {
        return (T)m_BlockData[T::GetID()][block.GetID()];
    }

    template<typename T>
    T& Chunk::AddBlockData(Block block)
    {
        m_BlockData[T::GetID()][block.GetID()] = (void*)T();
        return GetBlockData<T>(block);
    }

    template<typename T>
    void Chunk::RemoveBlockData(Block block)
    {
        m_BlockData[T::GetID()].erase(block.GetID());
    }
}
