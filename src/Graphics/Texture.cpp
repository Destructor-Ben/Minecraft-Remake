#include "Texture.h"

namespace Minecraft
{
    Texture::Texture()
    {
        glGenTextures(1, &m_ID);
        SetSWrappingMode(GL_CLAMP_TO_EDGE);
        SetTWrappingMode(GL_CLAMP_TO_EDGE);
        SetMinFilter(GL_NEAREST);
        SetMagFilter(GL_NEAREST);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_ID);
    }

    void Texture::SetSWrappingMode(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
        Unbind();
    }

    void Texture::SetTWrappingMode(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
        Unbind();
    }

    void Texture::SetWrappingModes(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
        Unbind();
    }

    void Texture::SetMinFilter(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        Unbind();
    }

    void Texture::SetMagFilter(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        Unbind();
    }

    void Texture::SetFilters(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        Unbind();
    }

    void Texture::GenerateMipMap()
    {
        Bind();
        glGenerateMipmap(GL_TEXTURE_2D);
        Unbind();
    }

    void Texture::SetData(byte* data, int width, int height, int format)
    {
        Bind();
        m_Width = width;
        m_Height = height;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        Unbind();
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
