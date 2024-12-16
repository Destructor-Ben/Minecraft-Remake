#pragma once

namespace Minecraft
{
    class CubeMap
    {
    public:
        CubeMap();
        ~CubeMap();

        void SetWrappingMode(GLenum mode);
        void SetFilters(GLenum filter);
        void SetFace(byte* data, int width, int height, uint face, int format = GL_RGB);

        void BindTextureUnit(byte textureUnit);
        void Bind();

        uint GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint m_ID = 0;
    };
}
