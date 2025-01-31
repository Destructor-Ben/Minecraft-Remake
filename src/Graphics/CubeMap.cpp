#include "CubeMap.h"

namespace Minecraft
{
    CubeMap::CubeMap()
    {
        glGenTextures(1, &m_ID);
        SetSWrappingMode(GL_CLAMP_TO_EDGE);
        SetTWrappingMode(GL_CLAMP_TO_EDGE);
        SetMinFilter(GL_LINEAR);
        SetMagFilter(GL_LINEAR);
    }

    CubeMap::~CubeMap()
    {
        glDeleteTextures(1, &m_ID);
    }

    void CubeMap::SetSWrappingMode(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode);
        Unbind();
    }

    void CubeMap::SetTWrappingMode(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode);
        Unbind();
    }

    void CubeMap::SetWrappingModes(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode);
        Unbind();
    }

    void CubeMap::SetMinFilter(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
        Unbind();
    }

    void CubeMap::SetMagFilter(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
        Unbind();
    }

    void CubeMap::SetFilters(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
        Unbind();
    }

    void CubeMap::GenerateMipMap()
    {
        Bind();
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        Unbind();
    }

    void CubeMap::SetFace(byte* data, int width, int height, uint face, int format)
    {
        Bind();
        glTexImage2D(face, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        Unbind();
    }

    void CubeMap::BindTextureUnit(byte textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        Bind();
    }

    void CubeMap::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
    }

    void CubeMap::Unbind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
