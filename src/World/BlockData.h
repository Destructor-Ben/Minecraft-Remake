#pragma once

#include "../Common.h"

#include "BlockType.h"

namespace Minecraft
{
    // TODO: temporary
    class BlockData
    {
    public:
        // TODO: we need some form of registry
        static int32 GetID();

        BlockType Type = BlockType::Air;
    };
}
