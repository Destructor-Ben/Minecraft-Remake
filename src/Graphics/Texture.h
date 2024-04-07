#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        // Important to remember that changing mip map in SetData doesn't change the filters
        void SetWrappingMode(GLenum mode);
        void SetFilters(GLenum filter);
        void SetData(uint8* data, int32 width, int32 height, int32 format = GL_RGB, bool mipMap = true);

        void BindTextureUnit(uint8 textureUnit) const;
        void Bind() const;

        static void Unbind();

        static Texture FromFile(const string& path, int32 format = GL_RGB, bool mipMap = true);

    private:
        uint32 m_ID = 0;
    };
}
