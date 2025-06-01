#include "ResourceManager.h"

#include "Game.h"
#include "Logger.h"
#include "Graphics/GL.h"

// Change this when doing an actual build so the assets are read from the same folder instead of 2 folders back
#define USE_DEV_ASSETS true

namespace Minecraft::Resources
{
    // Cache graphics resources that are requested multiple times
    // TODO: what if we modify some of them? should include an option to cache when requesting
    // Also needs options to add to caches in case these are made manually
    unordered_map <string, shared_ptr<Texture>> TextureCache = { };
    unordered_map <string, shared_ptr<Shader>> ShaderCache = { };
    unordered_map <string, shared_ptr<VertexShader>> VertexShaderCache = { };
    unordered_map <string, shared_ptr<FragmentShader>> FragmentShaderCache = { };

    string GetResourcePath(string path)
    {
        #if USE_DEV_ASSETS
        return "../../" + path;
        #else
        return path;
        #endif
    }

    string RequestResourceText(string path)
    {
        std::ifstream stream(GetResourcePath(path));
        if (stream.fail())
            Logger::Throw("Failed to load resource at path: " + path);

        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        return buffer.str();
    }

    vector <byte> RequestResourceBytes(string path)
    {
        std::ifstream stream(GetResourcePath(path), std::ios::binary);
        if (stream.fail())
            Logger::Throw("Failed to load resource at path: " + path);

        vector <byte> bytes((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        stream.close();
        return bytes;
    }

    // Used in a shared ptr to delete the image data from stbi automatically
    void ImageDeleter(byte* data)
    {
        stbi_image_free(data);
    }

    ImageData RequestImageData(string path)
    {
        // Get the data
        path = "assets/textures/" + path + ".png";
        auto compressedData = RequestResourceBytes(path);

        // Load it and calculate the format
        int width;
        int height;
        int channels;
        byte* data = stbi_load_from_memory(compressedData.data(), (int)compressedData.size(), &width, &height, &channels, 0);
        int format = channels == 4 ? GL_RGBA : GL_RGB;

        // Validate the data
        if (!data)
            Logger::Throw("Failed to load texture at path: " + path);

        // Turn into shared ptr with a deleter
        auto ptr = shared_ptr<byte>(data, ImageDeleter);

        return { width, height, format, ptr };
    }

    shared_ptr <Texture> RequestTexture(string path)
    {
        if (TextureCache.contains(path))
            return TextureCache[path];

        // Load the texture
        auto image = RequestImageData(path);

        // Set the data
        auto texture = make_shared<Texture>();
        texture->SetData(image.Data.get(), image.Width, image.Height, image.Format);
        TextureCache[path] = texture;

        return texture;
    }

    shared_ptr <Shader> RequestShader(string path)
    {
        if (ShaderCache.contains(path))
            return ShaderCache[path];

        auto vert = RequestVertexShader(path);
        auto frag = RequestFragmentShader(path);
        auto shader = make_shared<Shader>(*vert, *frag);

        ShaderCache[path] = shader;
        return shader;
    }

    shared_ptr <VertexShader> RequestVertexShader(string path)
    {
        if (VertexShaderCache.contains(path))
            return VertexShaderCache[path];

        path = "assets/shaders/" + path + ".vert";
        string shaderCode = RequestResourceText(path);
        auto shader = make_shared<VertexShader>(shaderCode);

        VertexShaderCache[path] = shader;
        return shader;
    }

    shared_ptr <FragmentShader> RequestFragmentShader(string path)
    {
        if (FragmentShaderCache.contains(path))
            return FragmentShaderCache[path];

        path = "assets/shaders/" + path + ".frag";
        string shaderCode = RequestResourceText(path);
        auto shader = make_shared<FragmentShader>(shaderCode);

        FragmentShaderCache[path] = shader;
        return shader;
    }
}
