#pragma once

#include "src/Common.h"
#include "src/World/Chunk.h"

namespace Minecraft
{
    class ChunkRenderer
    {
    public:
        ChunkRenderer();
        ~ChunkRenderer();

        std::vector<Quad> GetChunkFaces(Chunk& chunk);
        void CreateChunkMesh(const std::vector<float>& vertices, const std::vector<uint32>& indices);
        void DeleteChunkMesh();

        void RenderChunk(const Chunk& chunk);

    private:
        std::unordered_map<Chunk*, Mesh&> m_ChunkMeshes;

        Texture& m_ChunkTexture;
        Shader& m_ChunkShader;
    };
}
