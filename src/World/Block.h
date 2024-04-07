#pragma once

#include "../Common.h"

#include "BlockData.h"

namespace Minecraft
{
    // Forward declare since we can't include Chunk.h since it includes this file
    class Chunk;

    // Blocks are essentially an index to actual data, similar to an ECS
    class Block
    {
    public:
        Chunk& Parent;

        // The offset from the chunk coordinates
        const uint8 LocalX;
        const uint8 LocalY;
        const uint8 LocalZ;

        Block(Chunk& chunk, uint8 localX, uint8 localY, uint8 localZ);

        // Index of the block used for accessing the arrays of data in the chunks
        uint16 GetID() const;

        // Global positions
        int32 GetX() const;
        int32 GetY() const;
        int32 GetZ() const;

        // TODO: make vec3i
        // TODO: possible make GetWorldPos and GetBlockPos which return vec3i and vec3
        vec3 GetPosition() const { return vec3((float)GetX(), (float)GetY(), (float)GetZ()); }

        BlockData& GetData();

        /*/
        template<typename T>
        T& GetData();

        template<typename T>
        T& AddData();

        template<typename T>
        void RemoveData();
        //*/
    };
}
