#pragma once

namespace Minecraft
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        Texture(const Texture& other) = delete;
        Texture(Texture&& other) = delete;
        Texture& operator =(const Texture& other) = delete;
        Texture& operator =(Texture&& other) = delete;

        void SetSWrappingMode(GLenum mode);
        void SetTWrappingMode(GLenum mode);
        void SetWrappingModes(GLenum mode);

        void SetMinFilter(GLenum filter);
        void SetMagFilter(GLenum filter);
        void SetFilters(GLenum filter);

        // Important to remember that generating a mip map doesn't set the mip map filters to actually use the mip map
        void GenerateMipMap();
        void SetData(byte* data, int width, int height, int format = GL_RGB);

        void BindTextureUnit(byte textureUnit);
        void Bind();

        uint GetID() const { return m_ID; }
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        static void Unbind();

    private:
        uint m_ID = 0;

        int m_Width = 0;
        int m_Height = 0;
    };
}
