#pragma once

#include "World/Blocks/SolidBlock.h"

namespace Minecraft
{
    class GrassBlock : public SolidBlock
    {
    public:
        GrassBlock(int textureIndex) : SolidBlock(textureIndex) { }

        // TODO: polymorphism isn't working and this isnt being called
        int GetTextureIndex(vec3i dir) override
        {
            if (dir.y > 0)
                return TextureIndex + 2;
            else if (dir.y < 0)
                return TextureIndex;
            else
                return TextureIndex + 1;
        }
    };
}