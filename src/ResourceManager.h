#pragma once

namespace Minecraft
{
    class Texture;
    class CubeMap;
    class Shader;
    class VertexShader;
    class FragmentShader;

    struct ImageData
    {
        int Width = 0;
        int Height = 0;
        int Format = 0;
        shared_ptr<byte> Data = nullptr;
    };

    // TODO: avoid copying assets and just directly access them from the root folder if a macro is enabled
    class ResourceManager
    {
    public:
        string RequestResourceText(string path);
        vector<byte> RequestResourceBytes(string path);
        ImageData RequestImageData(string path);

        shared_ptr<Texture> RequestTexture(string path);
        shared_ptr<CubeMap> RequestCubeMap(string path);
        shared_ptr<Shader> RequestShader(string path);
        shared_ptr<VertexShader> RequestVertexShader(string path);
        shared_ptr<FragmentShader> RequestFragmentShader(string path);

    private:
        // Used in a shared ptr to delete the image data from stbi automatically
        static void ImageDeleter(byte* data);

        // Order in this matters!
        vector<string> m_CubeMapFaceNames = {
            "right",
            "left",
            "top",
            "bottom",
            "front",
            "back",
        };

        // Cache graphics resources that are requested multiple times
        unordered_map<string, shared_ptr<Texture>> m_Textures = { };
        unordered_map<string, shared_ptr<CubeMap>> m_CubeMaps = { };
        unordered_map<string, shared_ptr<Shader>> m_Shaders = { };
        unordered_map<string, shared_ptr<VertexShader>> m_VertexShaders = { };
        unordered_map<string, shared_ptr<FragmentShader>> m_FragmentShaders = { };
    };
}
