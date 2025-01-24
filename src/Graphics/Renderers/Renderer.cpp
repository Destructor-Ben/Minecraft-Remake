#include "Renderer.h"

#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/DebugMaterial.h"
#include "Graphics/Renderers/ChunkRenderer.h"

namespace Minecraft
{
    Renderer::Renderer()
    {
        InitDebugMeshes();
    }

    void Renderer::Update()
    {
        if (SceneCamera != nullptr)
            SceneCamera->Update();
    }

    void Renderer::PreRender()
    {
        if (DrawWireframes)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Renderer::PostRender()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void Renderer::DrawMesh(const Mesh& mesh, mat4 transform)
    {
        // Frustum culling
        if (mesh.Bounds.has_value() && !SceneCamera->Frustum.ContainsBounds(mesh.Bounds.value()))
            return;

        mesh.Draw(SceneCamera->ProjectionViewMatrix * transform);
    }

    void Renderer::InitDebugMeshes()
    {
        // Create debug material
        auto shader = RequestShader("debug");
        m_DebugMaterial = make_shared<DebugMaterial>(shader);

        // Create point mesh
        float pointVertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };

        auto pointVertexBuffer = make_shared<VertexBuffer>();
        pointVertexBuffer->SetData(pointVertices, 4 * 3);

        uint pointIndices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        auto pointIndexBuffer = make_shared<IndexBuffer>();
        pointIndexBuffer->SetData(pointIndices, 6);

        auto pointVertexArray = make_shared<VertexArray>();
        pointVertexArray->PushFloat(3);
        pointVertexArray->AddBuffer(pointVertexBuffer);

        m_DebugPointMesh = make_shared<Mesh>(pointVertexArray);
        m_DebugPointMesh->AddMaterial(m_DebugMaterial, pointIndexBuffer);

        // Create bounds mesh
        float boundsVertices[] = {
            0.0f, 0.0f, 0.0f, // 0
            1.0f, 0.0f, 0.0f, // 1
            1.0f, 1.0f, 0.0f, // 2
            0.0f, 1.0f, 0.0f, // 3
            0.0f, 0.0f, 1.0f, // 4
            1.0f, 0.0f, 1.0f, // 5
            1.0f, 1.0f, 1.0f, // 6
            0.0f, 1.0f, 1.0f, // 7
        };

        auto boundsVertexBuffer = make_shared<VertexBuffer>();
        boundsVertexBuffer->SetData(boundsVertices, 8 * 3);

        uint boundsIndices[] = {
            // Front face
            0, 2, 1,
            0, 3, 2,
            // Back face
            4, 5, 6,
            4, 6, 7,
            // Left face
            0, 7, 3,
            0, 4, 7,
            // Right face
            1, 6, 5,
            1, 2, 6,
            // Top face
            3, 6, 2,
            3, 7, 6,
            // Bottom face
            0, 5, 4,
            0, 1, 5,
        };

        auto boundsIndexBuffer = make_shared<IndexBuffer>();
        boundsIndexBuffer->SetData(boundsIndices, 6 * 2 * 3);

        auto boundsVertexArray = make_shared<VertexArray>();
        boundsVertexArray->PushFloat(3);
        boundsVertexArray->AddBuffer(boundsVertexBuffer);

        m_DebugBoundsMesh = make_shared<Mesh>(boundsVertexArray);
        m_DebugBoundsMesh->AddMaterial(m_DebugMaterial, boundsIndexBuffer);
    }

    void Renderer::DebugDrawPoint(vec3 point, vec3 color)
    {
        // Move to the position and face the camera
        auto transform = glm::translate(point);
        transform *= toMat4(glm::quatLookAt(SceneCamera->GetForwardVector(), vec3(0, 1, 0)));

        m_DebugMaterial->Color = color;
        m_DebugMaterial->DrawCircle = true;
        DrawMesh(*m_DebugPointMesh, transform);
    }

    void Renderer::DebugDrawBounds(BoundingBox bounds, vec3 color)
    {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        auto transform = glm::translate(bounds.Origin);
        transform *= glm::scale(bounds.Size);

        m_DebugMaterial->Color = color;
        m_DebugMaterial->DrawCircle = false;
        DrawMesh(*m_DebugBoundsMesh, transform);

        glEnable(GL_CULL_FACE);
        if (!DrawWireframes)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        for (int i = 0; i < m_CubeMapFaceNames.size(); i++)
        {
            // Load the data
            string facePath = "assets/textures/" + path + "/" + m_CubeMapFaceNames[i] + ".png";
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
        CubeMap::Unbind();
    }
}
