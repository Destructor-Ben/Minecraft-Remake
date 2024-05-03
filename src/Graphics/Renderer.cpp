#include "Renderer.h"
#include "src/Game.h"

namespace Minecraft
{
    Renderer::~Renderer()
    {
        for (auto resource: m_GraphicsResources)
        {
            delete resource;
        }
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

    void Renderer::DrawMesh(const Mesh& mesh, const mat4& transform) const
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

    void Renderer::TrackGraphicsResource(GraphicsResource* resource)
    {
        m_GraphicsResources.push_back(resource);
    }

    // TODO: make these use exceptions and error messages properly
    // TODO: possibly remove the hasAlpha option and don't choose channels, and chose the format based on the numver of channels
    Texture& Renderer::RequestTexture(string path, bool hasAlpha, bool mipMap)
    {
        path = "assets/textures/" + path + ".png";

        if (m_Textures.contains(path))
            return *m_Textures[path];

        // Load the texture
        int32 format = hasAlpha ? GL_RGBA : GL_RGB;
        int32 width, height, channels;
        uint8* data = stbi_load(path.c_str(), &width, &height, &channels, hasAlpha ? 4 : 3);
        if (!data)
            Logger->Error("Failed to load texture at path: " + path);

        // Set the data
        auto* texture = new Texture();
        texture->SetData(data, width, height, format, mipMap);
        texture->SetFilters(GL_NEAREST);  // prevents smoothing of low-res textures
        stbi_image_free(data);

        TrackGraphicsResource(texture);
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

        TrackGraphicsResource(shader);
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

        TrackGraphicsResource(shader);
        m_VertexShaders[path] = shader;
        return *shader;
    }

    FragmentShader& Renderer::RequestFragmentShader(string path)
    {
        path = "assets/shaders/" + path + ".frag";

        if (m_FragmentShaders.contains(path))
            return *m_FragmentShaders[path];

        std::ifstream stream(path);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        auto shader = new FragmentShader(buffer.str());

        TrackGraphicsResource(shader);
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
}
