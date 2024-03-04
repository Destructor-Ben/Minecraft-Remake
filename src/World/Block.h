#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Block {
        private:
            uint16_t BlockType;  // 0 = air, 1 = grass
        public:
            uint16_t GetBlockType();
    };
}