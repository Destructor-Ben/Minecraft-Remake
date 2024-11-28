#include "ChunkRenderer.h"

#include "Graphics/GL.h"
#include "Graphics/Renderers/ChunkMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/Chunk.h"
#include "World/World.h"

namespace Minecraft
{
    ChunkRenderer::ChunkRenderer(class Renderer& renderer) : m_Renderer(renderer)
    {
        auto texture = m_Renderer.RequestTexture("chunk");
        auto shader = m_Renderer.RequestShader("shader");
        m_ChunkMaterial = make_shared<ChunkMaterial>(shader);
        m_ChunkMaterial->Texture = texture;
    }

    void ChunkRenderer::RenderChunk(Chunk& chunk)
    {
        Transform transform { };
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

        auto vertices = Quad::ToVertices(faces);
        SetMeshData(chunk, Vertex::ToFloats(vertices), Vertex::ToIndices(vertices));
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

        auto bounds = BoundingBox(chunk.GetWorldPos(), vec3(chunk.Size));
        auto mesh = make_shared<Mesh>(vertexArray, bounds);
        mesh->AddMaterial(m_ChunkMaterial, indexBuffer);
        m_ChunkMeshes[&chunk] = mesh;
    }

    void ChunkRenderer::SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices)
    {
        m_ChunkMeshes[&chunk]->Vertices->GetBuffer()->SetData(vertices);
        m_ChunkMeshes[&chunk]->GetIndexBuffer(m_ChunkMaterial)->SetData(indices);
    }

    void ChunkRenderer::AddFaceInDirection(Chunk& chunk, Block& block, vector <Quad>& faces, vec3i dir, quat rotation)
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
        float sizeOfOneTexture = 1.0f / 8.0f; // 8 comes from 128 (atlas size) / 16 (one texture size)
        face.UVMultiplier = vec2(sizeOfOneTexture);// TODO: don't hardcode texture size!

        if (block.GetData().Type == BlockType::Dirt)
            face.UVOffset = vec2(2 * sizeOfOneTexture, 1.0f - sizeOfOneTexture);
        else
            face.UVOffset = vec2(sizeOfOneTexture, 1.0f - sizeOfOneTexture);

        faces.push_back(face);
    }

    // TODO: fix rotations - might be an issue with transforms
    vector <Quad> ChunkRenderer::GetChunkFaces(Chunk& chunk)
    {
        const float Degrees180 = glm::radians(180.0f);
        const float Degrees90 = glm::radians(90.0f);

        vector <Quad> faces;

        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    auto block = chunk.GetBlock(x, y, z);
                    if (block.GetData().Type == BlockType::Air)
                        continue;

                    // TODO: store these rotations
                    // Top and bottom
                    AddFaceInDirection(chunk, block, faces, vec3i(0, 1, 0), vec3(0, 0, 0));
                    AddFaceInDirection(chunk, block, faces, vec3i(0, -1, 0), vec3(Degrees180, Degrees180, 0));

                    // Left and right
                    AddFaceInDirection(chunk, block, faces, vec3i(1, 0, 0), quat(vec3(Degrees90, 0, 0)) * quat(vec3(0, 0, -Degrees90)));
                    AddFaceInDirection(chunk, block, faces, vec3i(-1, 0, 0), quat(vec3(Degrees90, 0, 0)) * quat(vec3(0, 0, Degrees90)));

                    // Front and back
                    AddFaceInDirection(chunk, block, faces, vec3i(0, 0, 1), vec3(Degrees90, 0, 0));
                    AddFaceInDirection(chunk, block, faces, vec3i(0, 0, -1), vec3(Degrees90, Degrees180, 0));
                }
            }
        }

        return faces;
    }

    // TODO: blocks such as grass and leaves will eventually have grey and will be tinted based on biome colour - olmarsh
    vec4 ChunkRenderer::GetFaceShading(vec3i dir)
    {
        float strength = 0;

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

}
