#include "Renderer.h"

#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/CameraFrustum.h"
#include "Graphics/GL.h"
#include "Graphics/Renderers/ChunkRenderer.h"

namespace Minecraft
{
    Renderer::Renderer()
    {
        // Have to initialize here because otherwise of init order
        ChunkRenderer = make_shared<class ChunkRenderer>(*this);
    }

    void Renderer::Update()
    {
        if (Camera != nullptr)
        {
            ViewMatrix = Camera->GetViewMatrix();
            ProjectionMatrix = Camera->GetProjectionMatrix();
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
        // TODO: use bounding box from mesh - needs to be added
        if (!Camera->GetFrustum().ContainsPoint(transform * vec4(0.0f, 0.0f, 0.0f, 1.0f)))
            return;

        mesh.Draw(ProjectionMatrix * ViewMatrix * transform);
    }

    shared_ptr <Texture> Renderer::RequestTexture(string path)
    {
        if (m_Textures.contains(path))
            return m_Textures[path];

        // Load the texture
        path = "assets/textures/" + path + ".png";
        auto compressedData = Instance->Resources->ReadResourceBytes(path);

        int width, height, channels;
        byte* data = stbi_load_from_memory(compressedData.data(), (int)compressedData.size(), &width, &height, &channels, 0);
        int format = channels == 4 ? GL_RGBA : GL_RGB;

        if (!data)
            Instance->Logger->Throw("Failed to load texture at path: " + path);

        // Set the data
        auto texture = make_shared<Texture>();
        texture->SetData(data, width, height, format);
        stbi_image_free(data);

        m_Textures[path] = texture;
        return texture;
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
