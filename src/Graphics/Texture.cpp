#include "Texture.h"

namespace Minecraft
{
    Texture::Texture()
    {
        glGenTextures(1, &m_ID);
        Bind();
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

    void Texture::SetData(uint8* data, int32 width, int32 height, int32 format, bool mipMap)
    {
        Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        if (mipMap)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::BindTextureUnit(uint8 textureUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        Bind();
    }

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture Texture::FromFile(const string& path, int32 format, bool mipMap)
    {
        int width, height, channels;
        uint8* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            Log("Failed to load texture at path: " + path);
            throw std::exception();
        }

        // TODO: creating the texture like this will delete it after it is returned
        Texture texture;
        texture.SetData(data, width, height, format, mipMap);
        stbi_image_free(data);
        return texture;
    }
}
