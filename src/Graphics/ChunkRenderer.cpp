#include "ChunkRenderer.h"

#include "src/Game.h"

namespace Minecraft
{
    ChunkRenderer::ChunkRenderer()
    {
        m_ChunkTexture = Renderer->RequestTexture("test");
        auto shader = Renderer->RequestShader("shader");
        m_ChunkMaterial = new Material(shader);
    }

    ChunkRenderer::~ChunkRenderer()
    {

    }

    // TODO: implement greedy meshing - flat, adjacent block faces use the same quad
    void Chunk::RegenerateMesh()
    {
        auto faces = GetFaces();

        // Generating the vertices
        if (faces.empty())
            return;

        // TODO: move this into a function in another file - renderer
        auto vertices = std::vector<float>();
        auto indices = std::vector<uint32>();
        for (Quad face : faces)
        {
            // Vertices
            // TODO: properly handle the rotation with the normal
            vec3 centre = face.Centre;
            vertices.push_back(centre.x - 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z - 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);

            vertices.push_back(centre.x + 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z - 0.5f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);

            vertices.push_back(centre.x - 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z + 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(centre.x + 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z + 0.5f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // Indices
            // TODO: use a vertex struct to make this maths less convoluted
            uint32 vertexCount = vertices.size() / 5;
            uint32 index0 = vertexCount - 4;
            uint32 index1 = vertexCount - 3;
            uint32 index2 = vertexCount - 2;
            uint32 index3 = vertexCount - 1;

            indices.push_back(index2);
            indices.push_back(index1);
            indices.push_back(index0);

            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index3);
        }
        // End TODO

        CreateMesh(vertices, indices);
    }

    void Chunk::CreateMesh(const std::vector<float>& vertices, const std::vector<uint32>& indices)
    {
        DeleteMesh();

        m_VertexBuffer = new VertexBuffer();
        m_VertexBuffer->SetData(vertices.data(), vertices.size());

        m_IndexBuffer = new IndexBuffer();
        m_IndexBuffer->SetData(indices.data(), indices.size());

        m_VertexArray = new VertexArray();
        m_VertexArray->Push(GL_FLOAT, 3);
        m_VertexArray->Push(GL_FLOAT, 2);
        m_VertexArray->AddBuffer(*m_VertexBuffer);

        m_Mesh = new Mesh(*m_VertexArray);

        m_Mesh->AddMaterial(m_Material, m_IndexBuffer);
    }

    void Chunk::DeleteMesh(Chunk* chunk)
    {
        if (!m_ChunkMeshes[chunk].m_VertexArray)
            delete m_Mesh;

        if (!m_VertexArray)
            delete m_VertexArray;

        if (!m_VertexBuffer)
            delete m_VertexBuffer;

        if (!m_IndexBuffer)
            delete m_IndexBuffer;

        if (!m_Material)
            delete m_Material;
    }

    std::vector<Quad> Chunk::GetFaces()
    {
        std::vector<Quad> faces;

        // TODO: make this work with neighboring chunks - don't forget to update nearby chunks
        // TODO: make this add faces properly
        for (int x = 0; x < Size; ++x)
        {
            for (int y = 0; y < Size; ++y)
            {
                for (int z = 0; z < Size; ++z)
                {
                    Block block = GetBlock(x, y, z);
                    if (block.GetData().Type == BlockType::Air)
                        continue;

                    Quad face{};
                    face.Centre = block.GetChunkPos();
                    face.Normal = vec3(0.0f, 1.0f, 0.0f);
                    faces.push_back(face);
                }
            }
        }

        return faces;
    }

    void ChunkRenderer::RenderChunk(const Chunk& chunk)
    {
        m_Texture->BindTextureUnit(0);
        m_Shader->Bind();
        m_Shader->SetUniform("uTexture", 0);

        Transform transform;
        transform.Position = vec3(X * Size, Y * Size, Z * Size);
        Renderer->DrawMesh(*m_Mesh, transform.GetTransformationMatrix());
    }
}
