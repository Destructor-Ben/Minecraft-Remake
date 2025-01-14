#include "ChunkRenderer.h"

#include "Game.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/ChunkMaterial.h"
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

        // Precompute these texture values
        m_BlockTextureSizeInUVCoords.x = (float)BlockTextureSize / m_ChunkTexture->GetWidth();
        m_BlockTextureSizeInUVCoords.y = (float)BlockTextureSize / m_ChunkTexture->GetHeight();
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
        vertexArray->PushFloat(3);
        vertexArray->AddBuffer(vertexBuffer);

        auto bounds = BoundingBox(chunk.GetWorldPos(), vec3(chunk.Size));
        auto mesh = make_shared<Mesh>(vertexArray, bounds);
        mesh->AddMaterial(m_ChunkMaterial, indexBuffer);
        m_ChunkMeshes[chunk.GetChunkPos()] = mesh;
    }

    void ChunkRenderer::SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices)
    {
        m_ChunkMeshes[chunk.GetChunkPos()]->Vertices->GetBuffer()->SetData(vertices, GL_DYNAMIC_DRAW);
        m_ChunkMeshes[chunk.GetChunkPos()]->GetIndexBuffer(m_ChunkMaterial)->SetData(indices, GL_DYNAMIC_DRAW);
    }

    vector <Quad> ChunkRenderer::GetChunkFaces(Chunk& chunk)
    {
        vector <Quad> faces;

        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    auto block = chunk.GetBlock(x, y, z);

                    switch (block.Data.Type->MeshType)
                    {
                        case BlockMeshType::None:
                            break;
                        case BlockMeshType::SolidCube:
                            SolidCubeMesh(block, faces);
                            break;
                        case BlockMeshType::GrassPlant:
                            GrassPlantMesh(block, faces);
                            break;
                    }
                }
            }
        }

        return faces;
    }

    void ChunkRenderer::SolidCubeMesh(Block& block, vector <Quad>& faces)
    {
        // Top and bottom
        AddFaceInDirection(block, faces, vec3i(0, 1, 0), m_BlockFaceDirections[0]);
        AddFaceInDirection(block, faces, vec3i(0, -1, 0), m_BlockFaceDirections[1]);

        // Left and right
        AddFaceInDirection(block, faces, vec3i(1, 0, 0), m_BlockFaceDirections[2]);
        AddFaceInDirection(block, faces, vec3i(-1, 0, 0), m_BlockFaceDirections[3]);

        // Front and back
        AddFaceInDirection(block, faces, vec3i(0, 0, 1), m_BlockFaceDirections[4]);
        AddFaceInDirection(block, faces, vec3i(0, 0, -1), m_BlockFaceDirections[5]);
    }

    void ChunkRenderer::GrassPlantMesh(Block& block, vector <Quad>& faces)
    {
        // TODO: should the backfaces mirror instead of being flipped?
        for (int i = 0; i < 4; ++i)
        {
            // TODO: proper direction
            vec3i dir = vec3i();

            Quad face { };
            face.Position = block.GetBlockPos();
            face.Rotation = m_GrassPlantFaceRotation * quat(glm::eulerAngleZ(Degrees90 * i));
            SetFaceTexture(face, dir, block.Data.Type->GetTextureCoords(dir));

            faces.push_back(face);
        }
    }

    void ChunkRenderer::AddFaceInDirection(Block& block, vector <Quad>& faces, vec3i dir, quat rotation)
    {
        // Getting other block
        auto otherBlockWorldPos = vec3i(block.GetWorldPos().x + dir.x, block.GetWorldPos().y + dir.y, block.GetWorldPos().z + dir.z);
        auto otherBlock = Instance->CurrentWorld->GetBlock(otherBlockWorldPos);

        // Only create faces next to transparent blocks
        bool isOtherBlockSolid = !otherBlock.has_value() || otherBlock.value().Data.Type->MeshType == BlockMeshType::SolidCube;
        auto otherBlockChunkPos = WorldToChunkPos(otherBlockWorldPos);
        bool isInWorldLimits = otherBlockChunkPos.y <= World::MaxHeight && otherBlockChunkPos.y >= World::MinHeight;
        // We want the top and bottom blocks of the world to have faces there otherwise it is ugly
        // So we only return if the adjacent block is in the world limits so the ones outside are treated as solid
        if (isOtherBlockSolid && isInWorldLimits)
            return;

        // Creating face
        Quad face { };
        face.Position = block.GetBlockPos();
        face.Position += vec3(dir) * 0.5f;
        face.Rotation = rotation;
        SetFaceTexture(face, dir, block.Data.Type->GetTextureCoords(dir));

        // Add the face
        faces.push_back(face);
    }

    void ChunkRenderer::SetFaceTexture(Quad& face, vec3i dir, vec2i textureCoords)
    {
        face.Shading = GetFaceTint(dir);

        // Calculate the UVs
        face.UVOffset = vec2(textureCoords.x * m_BlockTextureSizeInUVCoords.x, 1.0f - (textureCoords.y + 1) * m_BlockTextureSizeInUVCoords.y);
        face.UVMultiplier = vec2(m_BlockTextureSizeInUVCoords.x, m_BlockTextureSizeInUVCoords.y);
    }

    // TODO: blocks such as grass and leaves will eventually have grey and will be tinted based on biome colour
    // TODO: make a deferred renderer that doesn't use shading in the mesh
    vec3 ChunkRenderer::GetFaceTint(vec3i dir)
    {
        // Shading for lighting
        // TODO: redo this system to allow for shading in non axis directions
        float strength = 1;

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

        return vec3(strength);
    }
}
