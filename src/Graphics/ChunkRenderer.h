#pragma once

#include "src/Common.h"
#include "src/World/Chunk.h"

namespace Minecraft
{
    // TODO: implement greedy meshing - flat, adjacent block faces use the same quad
    class ChunkRenderer
    {
    public:
        ChunkRenderer();

        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

    private:
        void CreateMesh(Chunk& chunk);
        void DeleteMesh(Chunk& chunk);
        Quad GetFaceInDirection(Chunk& chunk, vec3 dir, vec3 rotation);
        std::vector<Quad> GetChunkFaces(Chunk& chunk);

        // TODO: make these references
        std::unordered_map<Chunk*, Mesh*> m_ChunkMeshes;
        std::unordered_map<Chunk*, VertexBuffer*> m_ChunkVertices;
        std::unordered_map<Chunk*, IndexBuffer*> m_ChunkIndices;

        Texture* m_ChunkTexture;
        Material* m_ChunkMaterial; // TODO: make a chunk material
        Shader* m_ChunkShader;
    };
}
