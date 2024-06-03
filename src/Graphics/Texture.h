#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        // Important to remember that generating a mip map in SetData doesn't set the mip map filters
        // Set wrapping mode and filters before data
        // TODO: make a regenerate mip map function
        void SetWrappingMode(GLenum mode);
        void SetFilters(GLenum filter);
        void SetData(uint8* data, int32 width, int32 height, int32 format = GL_RGB, bool mipMap = true);

        void BindTextureUnit(uint8 textureUnit) const;
        void Bind() const;

        static void Unbind();

    private:
        uint32 m_ID = 0;
    };
}
