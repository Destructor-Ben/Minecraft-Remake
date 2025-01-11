#pragma once

#include "World/Blocks/SolidBlock.h"

namespace Minecraft
{
    class WoodBlock : public SolidBlock
    {
    public:
        WoodBlock(vec2i textureCoords) : SolidBlock(textureCoords) { }

        vec2i GetTextureCoords(vec3i dir) override
        {
            if (dir.y == 0)
                return TextureCoords;
            else
                return TextureCoords + vec2i(1, 0);
        }
    };
}
