#include "ResourceManager.h"

#include "Game.h"
#include "LogManager.h"
#include "Graphics/GL.h"

namespace Minecraft
{
    string ResourceManager::RequestResourceText(string path)
    {
        std::ifstream stream(path);
        if (stream.fail())
            Instance->Logger->Throw("Failed to load resource at path: " + path);

        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        return buffer.str();
    }

    vector <byte> ResourceManager::RequestResourceBytes(string path)
    {
        std::ifstream stream(path, std::ios::binary);
        if (stream.fail())
            Instance->Logger->Throw("Failed to load resource at path: " + path);

        vector <byte> bytes((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        stream.close();
        return bytes;
    }

    ImageData ResourceManager::RequestImageData(string path)
    {
        // Get the data
        auto compressedData = Instance->Resources->RequestResourceBytes(path);

        // Load it and calculate the format
        int width;
        int height;
        int channels;
        byte* data = stbi_load_from_memory(compressedData.data(), (int)compressedData.size(), &width, &height, &channels, 0);
        int format = channels == 4 ? GL_RGBA : GL_RGB;

        // Validate the data
        if (!data)
            Instance->Logger->Throw("Failed to load texture at path: " + path);

        // Turn into shared ptr with a deleter
        auto ptr = shared_ptr<byte>(data, ResourceManager::ImageDeleter);

        return { width, height, format, ptr };
    }

    void ResourceManager::ImageDeleter(byte* data)
    {
        stbi_image_free(data);
    }

    shared_ptr <Texture> ResourceManager::RequestTexture(string path)
    {
        if (m_Textures.contains(path))
            return m_Textures[path];

        // Load the texture
        path = "assets/textures/" + path + ".png";
        auto image = RequestImageData(path);

        // Set the data
        auto texture = make_shared<Texture>();
        texture->SetData(image.Data.get(), image.Width, image.Height, image.Format);
        m_Textures[path] = texture;

        return texture;
    }

    shared_ptr <CubeMap> ResourceManager::RequestCubeMap(string path)
    {
        if (m_CubeMaps.contains(path))
            return m_CubeMaps[path];

        // Create the cubemap
        auto cubeMap = make_shared<CubeMap>();
        m_CubeMaps[path] = cubeMap;

        // Disable image flipping (I don't know why but I need to)
        stbi_set_flip_vertically_on_load(false);

        // Set the data for each face
        for (int i = 0; i < m_CubeMapFaceNames.size(); i++)
        {
            // Load the data
            string facePath = "assets/textures/" + path + "/" + m_CubeMapFaceNames[i] + ".png";
            auto image = RequestImageData(facePath);

            // Set the data
            cubeMap->SetFace(image.Data.get(), image.Width, image.Height, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, image.Format);
        }

        // Re-enable flipping
        stbi_set_flip_vertically_on_load(true);

        return cubeMap;
    }

    shared_ptr <Shader> ResourceManager::RequestShader(string path)
    {
        if (m_Shaders.contains(path))
            return m_Shaders[path];

        auto vert = RequestVertexShader(path);
        auto frag = RequestFragmentShader(path);
        auto shader = make_shared<Shader>(*vert, *frag);

        m_Shaders[path] = shader;
        return shader;
    }

    shared_ptr <VertexShader> ResourceManager::RequestVertexShader(string path)
    {
        if (m_VertexShaders.contains(path))
            return m_VertexShaders[path];

        path = "assets/shaders/" + path + ".vert";
        string shaderCode = Instance->Resources->RequestResourceText(path);
        auto shader = make_shared<VertexShader>(shaderCode);

        m_VertexShaders[path] = shader;
        return shader;
    }

    shared_ptr <FragmentShader> ResourceManager::RequestFragmentShader(string path)
    {
        if (m_FragmentShaders.contains(path))
            return m_FragmentShaders[path];

        path = "assets/shaders/" + path + ".frag";
        string shaderCode = Instance->Resources->RequestResourceText(path);
        auto shader = make_shared<FragmentShader>(shaderCode);

        m_FragmentShaders[path] = shader;
        return shader;
    }
}
