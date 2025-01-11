#pragma once

#include "World/Blocks/SolidBlock.h"

namespace Minecraft
{
    class GrassBlock : public SolidBlock
    {
    public:
        GrassBlock(vec2i textureCoords) : SolidBlock(textureCoords) { }

        vec2i GetTextureCoords(vec3i dir) override
        {
            if (dir.y > 0)
                return TextureCoords + vec2i(2, 0);
            else if (dir.y < 0)
                return TextureCoords;
            else
                return TextureCoords + vec2i(1, 0);
        }
    };
}
