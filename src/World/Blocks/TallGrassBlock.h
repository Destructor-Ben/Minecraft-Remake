#pragma once

#include "World/BlockType.h"

namespace Minecraft
{
    class TallGrassBlock : public BlockType
    {
    public:
        TallGrassBlock(int textureIndex)
        {
            TextureIndex = textureIndex;
            IsTransparent = true;
        }
    };
}
