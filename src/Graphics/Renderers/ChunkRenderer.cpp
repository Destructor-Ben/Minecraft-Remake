#include "ChunkRenderer.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/ChunkMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/Chunk.h"
#include "World/World.h"

namespace Minecraft
{
    ChunkRenderer::ChunkRenderer()
    {
        auto shader = Instance->Resources->RequestShader("chunk");
        m_ChunkTexture = Instance->Resources->RequestTexture("chunk");
        m_ChunkMaterial = make_shared<ChunkMaterial>(shader);
        m_ChunkMaterial->ChunkTexture = m_ChunkTexture;

        // Precompute these texture values
        m_BlockTextureSizeInUVCoords.x = (float)BlockTextureSize / m_ChunkTexture->GetWidth();
        m_BlockTextureSizeInUVCoords.y = (float)BlockTextureSize / m_ChunkTexture->GetHeight();
    }

    void ChunkRenderer::RenderDebugChunkBorders()
    {
        auto playerPos = Instance->CurrentWorld->PlayerCamera.Position;
        auto playerChunkPos = WorldToChunkPos(playerPos);
        if (!m_ChunkMeshes.contains(playerChunkPos))
            return;

        auto chunkBounds = m_ChunkMeshes.at(playerChunkPos)->Bounds;
        Instance->Graphics->DebugDrawBounds(chunkBounds.value(), glm::translate(ChunkToWorldPos(playerChunkPos)), vec3(1, 1, 0));
    }

    void ChunkRenderer::RenderChunks(const vector<Chunk*>& chunks)
    {
        // Remesh the chunks waiting in the queue (one at a time)
        // TODO: multi-thread
        // TODO: for some reason, this causes holes while generating chunks - might be an issue with the world generator
        while (!m_ChunkRemeshQueue.empty())
        {
            auto* chunk = m_ChunkRemeshQueue.pop();
            RegenerateMesh(*chunk);
        }

        // Render the chunks
        for (auto* chunk : chunks)
        {
            RenderChunk(*chunk);
        }
    }

    void ChunkRenderer::RenderChunk(Chunk& chunk)
    {
        if (!m_ChunkMeshes.contains(chunk.GetChunkPos()))
            return;

        if (m_IsChunkMeshEmpty.at(chunk.GetChunkPos()))
            return;

        Instance->Graphics->DrawMesh(*m_ChunkMeshes[chunk.GetChunkPos()], glm::translate(chunk.GetWorldPos()));
    }

    void ChunkRenderer::QueueMeshRegen(Chunk& chunk, int priority)
    {
        m_ChunkRemeshQueue.push(&chunk, priority);
    }

    void ChunkRenderer::RegenerateMesh(Chunk& chunk)
    {
        m_IsChunkMeshEmpty[chunk.GetChunkPos()] = false;

        if (!m_ChunkMeshes.contains(chunk.GetChunkPos()))
            CreateMesh(chunk);

        // TODO: preallocate size
        // - Possibly just estimate with say 50% of the max faces
        //   - Definitely profile
        // - Track the number of quads in each chunk and preallocate that when a mesh is regenerated
        //   - This only helps when breaking blocks however
        // TODO: reuse this vector to avoid reallocating it - how to do with multithreading?
        // - If I do that, then it will be much easier to prellocate the size
        auto faces = vector<Quad>();
        GetChunkFaces(chunk, faces);

        if (faces.empty())
        {
            m_IsChunkMeshEmpty[chunk.GetChunkPos()] = true;
            return;
        }

        // TODO: reuse this vector to avoid reallocating it - how to do with multithreading?
        auto vertices = vector<float>();
        auto indices = vector<uint>();
        Quad::ToRawData(faces, vertices, indices);
        SetMeshData(chunk, vertices, indices);
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

        auto bounds = BoundingBox(vec3(0), vec3(Chunk::Size));
        auto mesh = make_shared<Mesh>(vertexArray, bounds);
        mesh->Materials[m_ChunkMaterial] = indexBuffer;
        m_ChunkMeshes[chunk.GetChunkPos()] = mesh;
    }

    void ChunkRenderer::SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices)
    {
        m_ChunkMeshes[chunk.GetChunkPos()]->Vertices->Buffers[0]->SetData(vertices, GL_DYNAMIC_DRAW);
        m_ChunkMeshes[chunk.GetChunkPos()]->Materials[m_ChunkMaterial]->SetData(indices, GL_DYNAMIC_DRAW);
    }

    void ChunkRenderer::GetChunkFaces(Chunk& chunk, vector <Quad>& faces)
    {
        for_block_in_chunk(x, y, z, {
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
        })
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
            vec3i dir = vec3i();

            Quad face { };
            face.Position = block.GetBlockPos();
            face.Rotation = m_GrassPlantFaceRotation * quat(glm::eulerAngleZ(Degrees90 * i));
            SetFaceTexture(face, dir, block.Data.Type->GetTextureCoords(dir));

            // Adjustment to make faces align with bounds
            face.Position += vec3(0.5);

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

        // Adjustment to make faces align with bounds
        face.Position += vec3(0.5);

        // Add the face
        faces.push_back(face);
    }

    void ChunkRenderer::SetFaceTexture(Quad& face, vec3i dir, vec2i textureCoords)
    {
        face.Shading = GetFaceTint(dir);

        // Calculate the UVs
        face.UVPosition = vec2(textureCoords.x * m_BlockTextureSizeInUVCoords.x, 1.0f - (textureCoords.y + 1) * m_BlockTextureSizeInUVCoords.y);
        face.UVScale = vec2(m_BlockTextureSizeInUVCoords.x, m_BlockTextureSizeInUVCoords.y);
    }

    vec3 ChunkRenderer::GetFaceTint(vec3i dir)
    {
        // Shading for lighting
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
