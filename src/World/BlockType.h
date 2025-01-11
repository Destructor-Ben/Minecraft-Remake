#pragma once

#include "World/BlockMeshType.h"

namespace Minecraft
{
    // Block types are handled as a datatype that contains all the info and settings for a specific block type
    // The instance data is handled in BlockData
    class BlockType
    {
    public:
        BlockMeshType MeshType = BlockMeshType::SolidCube;
        vec2i TextureCoords = vec2i(0);

        // TODO: IsReplaceableInWorldGen
        // TODO: CollisionType

        virtual ~BlockType() = default;

        virtual vec2i GetTextureCoords(vec3i dir) { return TextureCoords; }
    };
}
