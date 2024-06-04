#include "ChunkRenderer.h"

#include "Game.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/Vertex.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"

namespace Minecraft
{
    ChunkRenderer::ChunkRenderer(class Renderer& renderer) : m_Renderer(renderer)
    {
        auto texture = m_Renderer.RequestTexture("stone");
        auto shader = m_Renderer.RequestShader("shader");
        m_ChunkMaterial = make_shared<ChunkMaterial>(shader);
        m_ChunkMaterial->Texture = texture;
    }

    void ChunkRenderer::RenderChunk(Chunk& chunk)
    {
        Transform transform = { };
        transform.Position = chunk.GetWorldPos();
        m_Renderer.DrawMesh(*m_ChunkMeshes[&chunk], transform.GetTransformationMatrix());
    }

    void ChunkRenderer::RegenerateMesh(Chunk& chunk)
    {
        if (!m_ChunkMeshes.contains(&chunk))
            CreateMesh(chunk);

        auto faces = GetChunkFaces(chunk);

        if (faces.empty())
            return;

        // Generating the vertices
        // TODO: move this into a function in another file - renderer
        auto vertices = vector<float32>();
        auto indices = vector<uint32>();
        for (Quad face : faces)
        {
            // Vertices
            // TODO: make a function to do this
            for (auto vertex : face.ToVertices())
            {
                vertices.push_back(vertex.Position.x);
                vertices.push_back(vertex.Position.y);
                vertices.push_back(vertex.Position.z);
                vertices.push_back(vertex.UV.x);
                vertices.push_back(vertex.UV.y);
                vertices.push_back(vertex.Shading.x);
                vertices.push_back(vertex.Shading.y);
                vertices.push_back(vertex.Shading.z);
                vertices.push_back(vertex.Shading.w);
            }

            // Indices
            // TODO: use a vertex struct to make this maths less convoluted
            uint32 vertexCount = vertices.size() / 9;
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

        // Set the mesh data
        SetMeshData(chunk, vertices, indices);
    }

    void ChunkRenderer::CreateMesh(Chunk& chunk)
    {
        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->PushFloat(2);
        vertexArray->PushFloat(4);
        vertexArray->AddBuffer(vertexBuffer);

        auto mesh = make_shared<Mesh>(vertexArray);
        m_ChunkMeshes[&chunk] = mesh;
        m_MaterialIDs[&chunk] = mesh->AddMaterial(m_ChunkMaterial, indexBuffer);
    }

    void ChunkRenderer::SetMeshData(Chunk& chunk, const vector<float32>& vertices, const vector<uint32>& indices)
    {
        m_ChunkMeshes[&chunk]->Vertices->GetBuffer()->SetData(vertices);
        m_ChunkMeshes[&chunk]->GetMaterial(m_MaterialIDs[&chunk]).Indices->SetData(indices);
    }

    void ChunkRenderer::AddFaceInDirection(Chunk& chunk, Block& block, vector<Quad>& faces, vec3i dir, vec3 rotation)
    {
        // Getting other block
        auto otherBlockWorldPos = vec3i(block.GetWorldPos().x + dir.x, block.GetWorldPos().y + dir.y, block.GetWorldPos().z + dir.z);
        auto otherBlock = TheWorld->GetBlock(otherBlockWorldPos);
        if (!otherBlock.has_value() || otherBlock.value().GetData().Type != BlockType::Air)
            return;

        // Creating face
        Quad face { };
        face.Position = block.GetBlockPos();
        face.Position += vec3(dir) * 0.5f;
        face.Rotation = rotation;
        face.Shading = GetFaceShading(dir);
        faces.push_back(face);
    }

    // TODO: fix rotations - might be an issue with transforms
    vector<Quad> ChunkRenderer::GetChunkFaces(Chunk& chunk)
    {
        vector<Quad> faces;

        for (int32 x = 0; x < Chunk::Size; ++x)
        {
            for (int32 y = 0; y < Chunk::Size; ++y)
            {
                for (int32 z = 0; z < Chunk::Size; ++z)
                {
                    auto block = chunk.GetBlock(x, y, z);
                    if (block.GetData().Type == BlockType::Air)
                        continue;

                    AddFaceInDirection(chunk, block, faces, vec3i(0, 1, 0), vec3(0, 0, 0));
                    AddFaceInDirection(chunk, block, faces, vec3i(0, -1, 0), vec3(180, 0, 0));

                    AddFaceInDirection(chunk, block, faces, vec3i(1, 0, 0), vec3(0, 0, -90));
                    AddFaceInDirection(chunk, block, faces, vec3i(-1, 0, 0), vec3(0, 0, 90));

                    AddFaceInDirection(chunk, block, faces, vec3i(0, 0, 1), vec3(90, 0, 0));
                    AddFaceInDirection(chunk, block, faces, vec3i(0, 0, -1), vec3(-90, 0, 0));
                }
            }
        }

        return faces;
    }

    // TODO: blocks such as grass and leaves will eventually have grey - olmarsh
    vec4 ChunkRenderer::GetFaceShading(vec3i dir)
    {
        float32 strength = 0;

        if (dir.x > 0)
            strength = 0.3f;
        else if (dir.y > 0)
            strength = 1.0f;
        else if (dir.z > 0)
            strength = 0.4f;
        else if (dir.x < 0)
            strength = 0.5f;
        else if (dir.y < 0)
            strength = 0.2f;
        else if (dir.z < 0)
            strength = 0.6f;

        return vec4(strength, strength, strength, 1.0f);
    }

    void ChunkRenderer::ChunkMaterial::Bind()
    {
        Material::Bind();

        Texture->BindTextureUnit(0);
        m_Shader->SetUniform("uTexture", 0);
    }
}
