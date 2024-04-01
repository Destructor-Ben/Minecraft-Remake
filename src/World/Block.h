#pragma once

#include "../Common.h"

namespace Minecraft
{
    // Blocks are essentially an index to actual data, similar to an ECS
    class Block
    {
    public:
        // TODO: see if we can make this a const Chunk& again, we couldn't before because of an issue with include loops
        const void* Parent;

        // The offset from the chunk coordinates
        const uint8 LocalX;
        const uint8 LocalY;
        const uint8 LocalZ;

        Block(const void* chunk, uint8 localX, uint8 localY, uint8 localZ);

        // Index of the block used for accessing the arrays of data in the chunks
        uint16 GetID() const;

        // Global positions
        int32 GetX() const;
        int32 GetY() const;
        int32 GetZ() const;

        template<typename T>
        T GetData();
    };
}
