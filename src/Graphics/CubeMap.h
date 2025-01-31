#pragma once

namespace Minecraft
{
    class CubeMap
    {
    public:
        CubeMap();
        ~CubeMap();

        CubeMap(const CubeMap& other) = delete;
        CubeMap(CubeMap&& other) = delete;
        CubeMap& operator =(const CubeMap& other) = delete;
        CubeMap& operator =(CubeMap&& other) = delete;

        void SetSWrappingMode(GLenum mode);
        void SetTWrappingMode(GLenum mode);
        void SetWrappingModes(GLenum mode);

        void SetMinFilter(GLenum filter);
        void SetMagFilter(GLenum filter);
        void SetFilters(GLenum filter);

        // Important to remember that generating a mip map doesn't set the mip map filters to actually use the mip map
        void GenerateMipMap();
        void SetFace(byte* data, int width, int height, uint face, int format = GL_RGB);

        void BindTextureUnit(byte textureUnit);
        void Bind();

        uint GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint m_ID = 0;
    };
}
