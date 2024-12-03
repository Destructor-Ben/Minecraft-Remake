#pragma once

#include "World/BlockType.h"

namespace Minecraft
{
    class SolidBlock : public BlockType
    {
    public:
        SolidBlock(int textureIndex)
        {
            TextureIndex = textureIndex;
            IsTransparent = false;
        }
    };
}
