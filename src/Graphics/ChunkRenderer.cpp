#include "ChunkRenderer.h"

#include "Vertex.h"
#include "src/Game.h"

namespace Minecraft
{
    ChunkRenderer::ChunkRenderer()
    {
        m_ChunkTexture = &Renderer->RequestTexture("stone");
        m_ChunkShader = &Renderer->RequestShader("shader");
        m_ChunkMaterial = new Material(*m_ChunkShader);
        Renderer->TrackGraphicsResource(m_ChunkMaterial);
    }

    void ChunkRenderer::RenderChunk(Chunk& chunk)
    {
        m_ChunkTexture->BindTextureUnit(0);
        m_ChunkShader->Bind();
        m_ChunkShader->SetUniform("uTexture", 0);

        Transform transform;
        transform.Position = vec3(chunk.X * Chunk::Size, chunk.Y * Chunk::Size, chunk.Z * Chunk::Size);
        Renderer->DrawMesh(*m_ChunkMeshes[&chunk], transform.GetTransformationMatrix());
    }

    void ChunkRenderer::RegenerateMesh(Chunk& chunk)
    {
        auto faces = GetChunkFaces(chunk);

        if (!m_ChunkMeshes.contains(&chunk))
            CreateMesh(chunk);

        if (faces.empty())
            return;

        // Generating the vertices
        // TODO: move this into a function in another file - renderer
        auto vertices = std::vector<float>();
        auto indices = std::vector<uint32>();
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

        // Create the mesh if it doesn't exist
        // TODO: make the renderer not own stuff, just use shared_ptr, that way we can delete the mesh

        // Set the mesh data
        m_ChunkVertices[&chunk]->SetData(vertices.data(), vertices.size());
        m_ChunkIndices[&chunk]->SetData(indices.data(), indices.size());
    }

    void ChunkRenderer::CreateMesh(Chunk& chunk)
    {
        auto vertexBuffer = new VertexBuffer();
        Renderer->TrackGraphicsResource(vertexBuffer);

        auto indexBuffer = new IndexBuffer();
        Renderer->TrackGraphicsResource(indexBuffer);

        auto vertexArray = new VertexArray();
        vertexArray->Push(GL_FLOAT, 3);
        vertexArray->Push(GL_FLOAT, 2);
        vertexArray->Push(GL_FLOAT, 4);
        vertexArray->AddBuffer(*vertexBuffer);
        Renderer->TrackGraphicsResource(vertexArray);

        auto mesh = new Mesh(*vertexArray);
        mesh->AddMaterial(m_ChunkMaterial, indexBuffer);
        Renderer->TrackGraphicsResource(mesh);

        m_ChunkMeshes[&chunk] = mesh;
        m_ChunkVertices[&chunk] = vertexBuffer;
        m_ChunkIndices[&chunk] = indexBuffer;
    }

    void ChunkRenderer::DeleteMesh(Chunk& chunk)
    {

    }

    void ChunkRenderer::AddFaceInDirection(Chunk& chunk, Block& block, std::vector<Quad>& faces, vec3 dir, vec3 rotation)
    {
        auto blockAbove = chunk.GetBlock(block.LocalX + (int)dir.x, block.LocalY + (int)dir.y, block.LocalZ + (int)dir.z);
        if (blockAbove.GetData().Type != BlockType::Air)
            return;

        Quad face;
        face.Position = block.GetChunkPos() + dir * 0.5f;
        face.Rotation = rotation;
        face.Shading = vec4(0.0f);
        // TODO: Move shading to function
        // TODO: Blocks such as grass and leaves will eventually have grey
        // textures and use shading to change their colour to suit environment
        if      (dir[0] == 1)  face.Shading = abs(vec4(0.3f,0.3f,0.3f,1.0f));
        else if (dir[1] == 1)  face.Shading = abs(vec4(1.0f,1.0f,1.0f,1.0f));
        else if (dir[2] == 1)  face.Shading = abs(vec4(0.4f,0.4f,0.4f,1.0f));
        else if (dir[0] == -1) face.Shading = abs(vec4(0.5f,0.5f,0.5f,1.0f));
        else if (dir[1] == -1) face.Shading = abs(vec4(0.1f,0.1f,0.1f,1.0f));
        else if (dir[2] == -1) face.Shading = abs(vec4(0.6f,0.6f,0.6f,1.0f));

        //face.Shading = abs(vec4(dir[2],dir[1],dir[0],1.0f)); // debugging colours
        faces.push_back(face);
    }

    std::vector<Quad> ChunkRenderer::GetChunkFaces(Chunk& chunk)
    {
        std::vector<Quad> faces;

        // TODO: make this work with neighboring chunks - don't forget to update nearby chunks
        // TODO: fix rotations - might be an issue with transforms
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    auto block = chunk.GetBlock(x, y, z);
                    if (block.GetData().Type == BlockType::Air)
                        continue;

                    AddFaceInDirection(chunk, block, faces, vec3(0, 1, 0), vec3(0, 0, 0));
                    AddFaceInDirection(chunk, block, faces, vec3(0, -1, 0), vec3(180, 0, 0));

                    AddFaceInDirection(chunk, block, faces, vec3(1, 0, 0), vec3(0, 0, -90));
                    AddFaceInDirection(chunk, block, faces, vec3(-1, 0, 0), vec3(0, 0, 90));

                    AddFaceInDirection(chunk, block, faces, vec3(0, 0, 1), vec3(90, 0, 0));
                    AddFaceInDirection(chunk, block, faces, vec3(0, 0, -1), vec3(-90, 0, 0));
                }
            }
        }

        return faces;
    }
}
