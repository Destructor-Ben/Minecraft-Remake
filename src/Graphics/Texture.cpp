#include "Texture.h"

namespace Minecraft
{
    Texture::Texture()
    {
        glGenTextures(1, &m_ID);
        Bind();
        SetWrappingMode(GL_CLAMP_TO_EDGE);
        SetFilters(GL_NEAREST);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_ID);
    }

    void Texture::SetWrappingMode(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    }

    void Texture::SetFilters(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    }

    void Texture::GenerateMipMap()
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::SetData(byte* data, int width, int height, int format)
    {
        Bind();
        m_Width = width;
        m_Height = height;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    void Texture::BindTextureUnit(byte textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        Bind();
    }

    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
