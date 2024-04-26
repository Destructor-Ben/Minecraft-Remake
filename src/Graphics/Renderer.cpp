#include "Renderer.h"

namespace Minecraft
{
    Renderer::~Renderer()
    {
        DeleteResources(m_Textures);
        DeleteResources(m_Shaders);
        DeleteResources(m_VertexShaders);
        DeleteResources(m_FragmentShaders);
    }

    void Renderer::Update()
    {
        if (m_Camera != nullptr)
        {
            ViewMatrix = m_Camera->GetViewMatrix();
            ProjectionMatrix = m_Camera->GetProjectionMatrix();
        }
        else
        {
            ViewMatrix = mat4(1.0f);
            ProjectionMatrix = mat4(1.0f);
        }
    }

    void Renderer::DrawMesh(const Mesh& mesh, const mat4& transform)
    {
        mesh.Vertices.Bind();

        for (int i = 0; i < mesh.MaterialCount; i++)
        {
            IndexBuffer& indexBuffer = *mesh.Indices[i];
            indexBuffer.Bind();

            Material& material = *mesh.Materials[i];
            material.Transform = ProjectionMatrix * ViewMatrix * transform;
            material.Bind();

            glDrawElements(GL_TRIANGLES, (int)indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    // TODO: make these use exceptions and error messages properly
    Texture& Renderer::RequestTexture(string path, int32 format, bool mipMap)
    {
        path = "assets/textures/" + path + ".png";

        if (m_Textures.contains(path))
            return *m_Textures[path];

        // Load the texture
        int width, height, channels;
        uint8* data = stbi_load(path.c_str(), &width, &height, &channels, 0); // TODO: possibly just make desired channels automatically set by format, and make the format justa bool for whether there is an alpha channel
        if (!data)
            throw std::exception(string("Failed to load texture at path: " + path).c_str());

        // Pick a default format
        if (format == -1)
        {
            if (channels == 4)
                format = GL_RGBA;
            else
                format = GL_RGB;
        }

        // Set the data
        auto* texture = new Texture();
        texture->SetData(data, width, height, format, mipMap);
        stbi_image_free(data);

        m_Textures[path] = texture;
        return *texture;
    }

    Shader& Renderer::RequestShader(string path)
    {
        if (m_Shaders.contains(path))
            return *m_Shaders[path];

        auto& vert = RequestVertexShader(path);
        auto& frag = RequestFragmentShader(path);
        auto* shader = new Shader(vert, frag);

        m_Shaders[path] = shader;
        return *shader;
    }

    VertexShader& Renderer::RequestVertexShader(string path)
    {
        path = "assets/shaders/" + path + ".vert";

        if (m_VertexShaders.contains(path))
            return *m_VertexShaders[path];

        std::ifstream stream(path);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        auto shader = new VertexShader(buffer.str());

        m_VertexShaders[path] = shader;
        return *shader;
    }

    FragmentShader& Renderer::RequestFragmentShader(string path)
    {
        path = "assets/textures/" + path + ".frag";

        if (m_FragmentShaders.contains(path))
            return *m_FragmentShaders[path];

        std::ifstream stream(path);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        auto shader = new FragmentShader(buffer.str());

        m_FragmentShaders[path] = shader;
        return *shader;
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

    template<typename T>
    void Renderer::DeleteResources(const std::unordered_map<string, T*>& resources)
    {
        for (auto resource : resources)
            delete resource.second;
    }
}
