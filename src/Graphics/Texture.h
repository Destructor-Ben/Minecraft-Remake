#pragma once

namespace Minecraft
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        // Important to remember that generating a mip map doesn't set the mip map filters
        // Set wrapping mode and filters before data if generating a mip map - However, I believe this is only a convention
        void SetWrappingMode(GLenum mode);
        void SetFilters(GLenum filter);
        void GenerateMipMap();
        void SetData(byte* data, int width, int height, int format = GL_RGB);

        void BindTextureUnit(byte textureUnit);
        void Bind();

        uint GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint m_ID = 0;
    };
}
