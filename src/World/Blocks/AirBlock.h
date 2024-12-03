#pragma once

#include "World/BlockType.h"

namespace Minecraft
{
    class AirBlock : public BlockType
    {
    public:
        AirBlock()
        {
            IsTransparent = true;
        }
    };
}
