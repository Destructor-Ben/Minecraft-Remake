#include "Block.h"

#include "Chunk.h"

namespace Minecraft
{
    Block::Block(const void* chunk, uint8 localX, uint8 localY, uint8 localZ) : Parent(chunk), LocalX(localX), LocalY(localY), LocalZ(localZ) { }

    uint16 Block::GetID() const
    {
        // TODO: test this - we want it to be a right hand system, so (0,0,0) is the bottom, back, left - does it even matter? it's only how it's laid out in memory
        return LocalX * Chunk::Size * Chunk::Size + LocalY * Chunk::Size + LocalZ;
    }

    int32 Block::GetX() const
    {
        return ((Chunk*)Parent)->X * Chunk::Size + LocalX;
    }

    int32 Block::GetY() const
    {
        return ((Chunk*)Parent)->Y * Chunk::Size + LocalY;
    }

    int32 Block::GetZ() const
    {
        return ((Chunk*)Parent)->Z * Chunk::Size + LocalZ;
    }

    template<typename T>
    T Block::GetData()
    {
        return ((Chunk*)Parent)->GetBlockData<T>(this);
    }
}
