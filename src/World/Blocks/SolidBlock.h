#pragma once

#include "World/BlockType.h"

namespace Minecraft
{
    class SolidBlock : public BlockType
    {
    public:
        SolidBlock(vec2i textureCoords)
        {
            TextureCoords = textureCoords;
        }
    };
}
