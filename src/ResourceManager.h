#pragma once

namespace Minecraft
{
    class Texture;
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
}

namespace Minecraft::Resources
{
    string GetResourcePath(string path);
    string RequestResourceText(string path);
    vector<byte> RequestResourceBytes(string path);
    ImageData RequestImageData(string path);

    shared_ptr<Texture> RequestTexture(string path);
    shared_ptr<Shader> RequestShader(string path);
    shared_ptr<VertexShader> RequestVertexShader(string path);
    shared_ptr<FragmentShader> RequestFragmentShader(string path);
}
