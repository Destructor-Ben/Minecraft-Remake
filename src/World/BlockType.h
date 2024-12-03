#pragma once

namespace Minecraft
{
    // Block types are handled as a datatype that contains all the info and settings for a specific block type
    // The instance data is handled in BlockData
    class BlockType
    {
    public:
        int TextureIndex = 0;
        bool IsTransparent = false;

        virtual ~BlockType() = default;

        virtual int GetTextureIndex(vec3i dir) { return TextureIndex; }
    };
}
