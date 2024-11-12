#pragma once

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
        class World* TheWorld = nullptr; // TODO: fix this mess - World isn't initialized yet! (also make the hierarchy of everything better - don't take everything in ctor, maybe just make a prepare method since global variables arent initted yet
        ChunkRenderer(class Renderer& renderer);

        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

    private:
        void CreateMesh(Chunk& chunk);
        void SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector<uint32>& indices);

        void AddFaceInDirection(Chunk& chunk, Block& block, vector<Quad>& faces, vec3i dir, quat rotation);
        vector<Quad> GetChunkFaces(Chunk& chunk);
        vec4 GetFaceShading(vec3i dir); // TODO: make a deferred renderer that doesn't use shading in the mesh

        class Renderer& m_Renderer;

        shared_ptr<ChunkMaterial> m_ChunkMaterial;
        // TODO: just use a hash of a chunk/chunk coordinate instead of using a ptr?
        unordered_map<Chunk*, shared_ptr<Mesh>> m_ChunkMeshes = { };
    };
}
