#pragma once

#include "../Common.h"

#include "Block.h"
#include "Entity.h"

namespace Minecraft
{
    class Chunk
    {
    public:
        static const uint8 Size = 10;

        Chunk();
        ~Chunk();

        Block& GetBlock(int32 x, int32 y, int32 z);

    private:
        Block* m_Blocks;
        std::vector<Entity> m_Entities;
    };
}
