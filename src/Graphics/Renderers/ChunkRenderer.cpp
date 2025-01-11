#include "ChunkRenderer.h"

#include "Game.h"
#include "Graphics/GL.h"
#include "Graphics/Renderers/ChunkMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/Chunk.h"
#include "World/World.h"

namespace Minecraft
{
    ChunkRenderer::ChunkRenderer()
    {
        auto shader = Instance->Graphics->RequestShader("chunk");
        m_ChunkTexture = Instance->Graphics->RequestTexture("chunk");
        m_ChunkMaterial = make_shared<ChunkMaterial>(shader);
        m_ChunkMaterial->ChunkTexture = m_ChunkTexture;
    }

    void ChunkRenderer::RenderChunk(Chunk& chunk)
    {
        if (!m_ChunkMeshes.contains(chunk.GetChunkPos()))
            RegenerateMesh(chunk);

        Transform transform { };
        transform.Position = chunk.GetWorldPos();
        Instance->Graphics->DrawMesh(*m_ChunkMeshes[chunk.GetChunkPos()], transform.GetTransformationMatrix());
    }

    void ChunkRenderer::RegenerateMesh(Chunk& chunk)
    {
        if (!m_ChunkMeshes.contains(chunk.GetChunkPos()))
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
        m_ChunkMeshes[chunk.GetChunkPos()] = mesh;
    }

    void ChunkRenderer::SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices)
    {
        m_ChunkMeshes[chunk.GetChunkPos()]->Vertices->GetBuffer()->SetData(vertices);
        m_ChunkMeshes[chunk.GetChunkPos()]->GetIndexBuffer(m_ChunkMaterial)->SetData(indices);
    }

    // TODO: create face function as well
    void ChunkRenderer::AddFaceInDirection(Chunk& chunk, Block& block, vector <Quad>& faces, vec3i dir, quat rotation)
    {
        // Getting other block
        auto otherBlockWorldPos = vec3i(block.GetWorldPos().x + dir.x, block.GetWorldPos().y + dir.y, block.GetWorldPos().z + dir.z);
        auto otherBlock = Instance->CurrentWorld->GetBlock(otherBlockWorldPos);

        // Only create faces next to transparent blocks
        bool isOtherBlockTransparent = !otherBlock.has_value() || !otherBlock.value().Data.Type->IsTransparent;
        auto otherBlockChunkPos = WorldToChunkPos(otherBlockWorldPos);
        bool isInWorldLimits = otherBlockChunkPos.y <= World::MaxHeight && otherBlockChunkPos.y >= World::MinHeight;
        // We want the top and bottom blocks of the world to have faces there otherwise it is ugly
        // So we only return if the adjacent block is in the world limits so the ones outside are treated as solid
        if (isOtherBlockTransparent && isInWorldLimits)
            return;

        // Creating face
        Quad face { };
        face.Position = block.GetBlockPos();
        face.Position += vec3(dir) * 0.5f;
        face.Rotation = rotation;
        face.Shading = GetFaceShading(dir);

        // Calculate the UVs
        float textureSizeX = (float)BlockTextureSize / m_ChunkTexture->GetWidth();
        float textureSizeY = (float)BlockTextureSize / m_ChunkTexture->GetHeight();
        face.UVMultiplier = vec2(textureSizeX, textureSizeY);

        int textureIndex = block.Data.Type->GetTextureIndex(dir);
        int texturesPerWidth = m_ChunkTexture->GetWidth() / BlockTextureSize;
        int textureX = textureIndex % texturesPerWidth;
        int textureY = textureIndex / texturesPerWidth;
        face.UVOffset = vec2(textureX * textureSizeX, 1.0f - (textureY + 1) * textureSizeY);

        // Add the face
        faces.push_back(face);
    }

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

                    // TODO: move this into BlockType
                    if (block.Data.Type == Blocks::TallGrass.get())
                    {
                        for (int i = 0; i < 4; ++i)
                        {
                            Quad face { };
                            face.Position = block.GetBlockPos();
                            face.Rotation = quat(vec3(Degrees90, Degrees90 / 2.0f + Degrees90 * i, 0.0f));
                            // TODO: shading
                            face.Shading = vec4(1.0f);

                            // Calculate the UVs
                            float textureSizeX = (float)BlockTextureSize / m_ChunkTexture->GetWidth();
                            float textureSizeY = (float)BlockTextureSize / m_ChunkTexture->GetHeight();
                            face.UVMultiplier = vec2(textureSizeX, textureSizeY);

                            int textureIndex = block.Data.Type->GetTextureIndex(vec3i(0));
                            // TODO: handle textureIndex >= width
                            face.UVOffset = vec2(textureIndex * textureSizeX, 1.0f - textureSizeY);

                            // Add the face
                            faces.push_back(face);
                        }

                        continue;
                    }

                    if (block.Data.Type->IsTransparent)
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
