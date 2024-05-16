#pragma once

#include "src/Common.h"
#include "src/World/Chunk.h"

namespace Minecraft
{
    class Renderer;
    
    // TODO: implement greedy meshing - flat, adjacent block faces use the same quad
    class ChunkRenderer
    {
    public:
        ChunkRenderer(Renderer& renderer);

        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

    private:
        void CreateMesh(Chunk& chunk);
        void SetMesh(Chunk& chunk, const std::vector<float32>& vertices, const std::vector<uint32>& indices);
        void DeleteMesh(Chunk& chunk);
        void AddFaceInDirection(Chunk& chunk, Block& block, std::vector<Quad>& faces, vec3i dir, vec3 rotation);
        std::vector<Quad> GetChunkFaces(Chunk& chunk);

        Renderer& m_Renderer;

        // TODO: make these references
        std::unordered_map<Chunk*, Mesh*> m_ChunkMeshes;
        std::unordered_map<Chunk*, VertexBuffer*> m_ChunkVertices;
        std::unordered_map<Chunk*, IndexBuffer*> m_ChunkIndices;

        Texture* m_ChunkTexture;
        Material* m_ChunkMaterial; // TODO: make a chunk material
        Shader* m_ChunkShader;
    };
}
