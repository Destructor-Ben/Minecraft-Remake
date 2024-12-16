#include "CubeMap.h"

namespace Minecraft
{
    CubeMap::CubeMap()
    {
        glGenTextures(1, &m_ID);
        Bind();
        SetWrappingMode(GL_CLAMP_TO_EDGE);
        SetFilters(GL_LINEAR);
    }

    CubeMap::~CubeMap()
    {
        glDeleteTextures(1, &m_ID);
    }

    void CubeMap::SetWrappingMode(GLenum mode)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mode);
    }

    void CubeMap::SetFilters(GLenum filter)
    {
        Bind();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
    }

    void CubeMap::SetFace(byte* data, int width, int height, uint face, int format)
    {
        Bind();
        glTexImage2D(face, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
