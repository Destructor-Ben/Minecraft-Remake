#pragma once

#include "src/Common.h"
#include "src/World/Chunk.h"

namespace Minecraft
{
    class Renderer;
    class World;

    // TODO: implement greedy meshing - flat, adjacent block faces use the same quad
    // TODO: clean up this mess
    class ChunkRenderer
    {
    public:
        World* TheWorld = nullptr; // TODO: fix this mess - World isn't initialized yet!
        ChunkRenderer(Renderer& renderer);

        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

    private:
        void CreateMesh(Chunk& chunk);
        void SetMesh(Chunk& chunk, const std::vector<float32>& vertices, const std::vector<uint32>& indices);

        void AddFaceInDirection(Chunk& chunk, Block& block, std::vector<Quad>& faces, vec3i dir, vec3 rotation);
        std::vector<Quad> GetChunkFaces(Chunk& chunk);

        Renderer& m_Renderer;

        // TODO: some of these can be unique ptrs
        unordered_map<Chunk*, shared_ptr<Mesh>> m_ChunkMeshes;
        unordered_map<Chunk*, shared_ptr<VertexBuffer>> m_ChunkVertices;
        unordered_map<Chunk*, shared_ptr<IndexBuffer>> m_ChunkIndices;

        shared_ptr<Texture> m_ChunkTexture;
        shared_ptr<Material> m_ChunkMaterial; // TODO: make a chunk material
        shared_ptr<Shader> m_ChunkShader;
    };
}
