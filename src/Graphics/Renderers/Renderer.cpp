#include "Renderer.h"

#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Renderers/ChunkRenderer.h"

namespace Minecraft
{
    void Renderer::Update()
    {
        if (SceneCamera != nullptr)
        {
            ViewMatrix = SceneCamera->GetViewMatrix();
            ProjectionMatrix = SceneCamera->GetProjectionMatrix();
        }
        else
        {
            ViewMatrix = mat4(1.0f);
            ProjectionMatrix = mat4(1.0f);
        }
    }

    void Renderer::DrawMesh(const Mesh& mesh, mat4 transform)
    {
        // Frustum culling
        // TODO: still has minor issues - maybe it's due to bad bounds checking
        if (mesh.Bounds.has_value() && !SceneCamera->GetFrustum().ContainsBounds(mesh.Bounds.value()))
            return;

        mesh.Draw(ProjectionMatrix * ViewMatrix * transform);
    }

    byte* Renderer::LoadImageData(string path, int& width, int& height, int& format)
    {
        // Get the data
        auto compressedData = Instance->Resources->ReadResourceBytes(path);

        // Load it and calculate the format
        int channels;
        byte* data = stbi_load_from_memory(compressedData.data(), (int)compressedData.size(), &width, &height, &channels, 0);
        format = channels == 4 ? GL_RGBA : GL_RGB;

        // Validate the data
        if (!data)
            Instance->Logger->Throw("Failed to load texture at path: " + path);

        return data;
    }

    shared_ptr <Texture> Renderer::RequestTexture(string path)
    {
        if (m_Textures.contains(path))
            return m_Textures[path];

        // Load the texture
        path = "assets/textures/" + path + ".png";
        int width, height, format;
        byte* data = LoadImageData(path, width, height, format);

        // Set the data
        auto texture = make_shared<Texture>();
        texture->SetData(data, width, height, format);
        m_Textures[path] = texture;

        // Free the memory
        stbi_image_free(data);

        return texture;
    }

    shared_ptr <CubeMap> Renderer::RequestCubeMap(string path)
    {
        if (m_CubeMaps.contains(path))
            return m_CubeMaps[path];

        // Create the cubemap
        auto cubeMap = make_shared<CubeMap>();
        m_CubeMaps[path] = cubeMap;

        // Disable image flipping (I don't know why but I need to)
        stbi_set_flip_vertically_on_load(false);

        // Set the data for each face
        for (auto [i, faceName] : views::enumerate(m_CubeMapFaceNames))
        {
            // Load the data
            string facePath = "assets/textures/" + path + "/" + faceName + ".png";
            int width, height, format;
            byte* data = LoadImageData(facePath, width, height, format);

            // Set the data
            cubeMap->SetFace(data, width, height, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, format);

            // Free the memory
            stbi_image_free(data);
        }

        // Re-enable flipping
        stbi_set_flip_vertically_on_load(true);

        return cubeMap;
    }

    shared_ptr <Shader> Renderer::RequestShader(string path)
    {
        if (m_Shaders.contains(path))
            return m_Shaders[path];

        auto vert = RequestVertexShader(path);
        auto frag = RequestFragmentShader(path);
        auto shader = make_shared<Shader>(*vert, *frag);

        m_Shaders[path] = shader;
        return shader;
    }

    shared_ptr <VertexShader> Renderer::RequestVertexShader(string path)
    {
        if (m_VertexShaders.contains(path))
            return m_VertexShaders[path];

        path = "assets/shaders/" + path + ".vert";
        string shaderCode = Instance->Resources->ReadResourceText(path);
        auto shader = make_shared<VertexShader>(shaderCode);

        m_VertexShaders[path] = shader;
        return shader;
    }

    shared_ptr <FragmentShader> Renderer::RequestFragmentShader(string path)
    {
        if (m_FragmentShaders.contains(path))
            return m_FragmentShaders[path];

        path = "assets/shaders/" + path + ".frag";
        string shaderCode = Instance->Resources->ReadResourceText(path);
        auto shader = make_shared<FragmentShader>(shaderCode);

        m_FragmentShaders[path] = shader;
        return shader;
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::UnbindAll()
    {
        VertexArray::Unbind();
        VertexBuffer::Unbind();
        IndexBuffer::Unbind();
        Shader::Unbind();
        Texture::Unbind();
    }
}
