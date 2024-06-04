#pragma once

#include "Common.h"

namespace Minecraft
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        // Important to remember that generating a mip map doesn't set the mip map filters
        // Set wrapping mode and filters before data if generating a mip map - However, I believe this is only a convention
        void SetWrappingMode(GLenum mode);
        void SetFilters(GLenum filter);
        void GenerateMipMap();
        void SetData(uint8* data, int32 width, int32 height, int32 format = GL_RGB);

        void BindTextureUnit(uint8 textureUnit);
        void Bind();

        uint32 GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint32 m_ID = 0;
    };
}
