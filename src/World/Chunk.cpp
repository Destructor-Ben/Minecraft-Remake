#include "Chunk.h"

#include "Block.h"

namespace Minecraft
{
    Block Chunk::GetBlock(uint8 localX, uint8 localY, uint8 localZ)
    {
        return Block();//Block((const Chunk&)(*this), localX, localY, localZ);
    }

    template<typename T>
    T Chunk::GetBlockData(Block block)
    {
        return nullptr;//BlocKData<T>[block.GetID()];
    }
}
