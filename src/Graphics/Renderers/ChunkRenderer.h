#pragma once

#include "Hash.h"

namespace Minecraft
{
    class Chunk;
    class ChunkMaterial;
    class Block;
    class Mesh;
    class Quad;
    class Renderer;
    class Texture;
    class VertexBuffer;
    class World;

    // TODO: implement greedy meshing - flat, adjacent block faces use the same quad
    class ChunkRenderer
    {
    public:
        const int BlockTextureSize = 16;

        ChunkRenderer();

        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

    private:
        void CreateMesh(Chunk& chunk);
        void SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices);

        void AddFaceInDirection(Chunk& chunk, Block& block, vector <Quad>& faces, vec3i dir, quat rotation);
        vector <Quad> GetChunkFaces(Chunk& chunk);
        vec4 GetFaceShading(vec3i dir); // TODO: make a deferred renderer that doesn't use shading in the mesh

        shared_ptr <Texture> m_ChunkTexture;
        shared_ptr <ChunkMaterial> m_ChunkMaterial;

        unordered_map <vec3i, shared_ptr<Mesh>> m_ChunkMeshes = { };
    };
}
