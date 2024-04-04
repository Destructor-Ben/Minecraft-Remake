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

    void Texture::SetData()
    {
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
}
