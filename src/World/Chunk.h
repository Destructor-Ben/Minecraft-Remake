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

    private:
        Block m_Blocks[Size * Size * Size];
        std::vector<Entity> m_Entities;
    };
}
