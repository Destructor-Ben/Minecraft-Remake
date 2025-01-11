#pragma once

#include "World/BlockType.h"

namespace Minecraft
{
    class TallGrassBlock : public BlockType
    {
    public:
        TallGrassBlock(vec2i textureCoords)
        {
            TextureCoords = textureCoords;
            MeshType = BlockMeshType::GrassPlant;
        }
    };
}
