#pragma once

#include "World/Blocks/SolidBlock.h"

namespace Minecraft
{
    class WoodBlock : public SolidBlock
    {
    public:
        WoodBlock(int textureIndex) : SolidBlock(textureIndex) { }

        int GetTextureIndex(vec3i dir) override
        {
            if (dir.y == 0)
                return TextureIndex;
            else
                return TextureIndex + 1;
        }
    };
}
