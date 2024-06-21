#pragma once

#include "Common.h"

#include "Graphics/Material.h"

namespace Minecraft
{
    class Chunk;
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
        World* TheWorld = nullptr; // TODO: fix this mess - World isn't initialized yet! (also make the hierarchy of everything better - don't take everything in ctor, maybe just make a prepare method since global variables arent initted yet
        ChunkRenderer(Renderer& renderer);

        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

    private:
        class ChunkMaterial : public Material
        {
        public:
            shared_ptr<Texture> Texture;

            ChunkMaterial(shared_ptr<Shader> shader) : Material(shader) { }

            void Bind() override;
        };

        void CreateMesh(Chunk& chunk);
        void SetMeshData(Chunk& chunk, const vector<float32>& vertices, const vector<uint32>& indices);

        void AddFaceInDirection(Chunk& chunk, Block& block, vector<Quad>& faces, vec3i dir, quat rotation);
        vector<Quad> GetChunkFaces(Chunk& chunk);
        vec4 GetFaceShading(vec3i dir);

        Renderer& m_Renderer;

        shared_ptr<ChunkMaterial> m_ChunkMaterial;
        // TODO: just use a hash of a chunk/chunk coordinate instead of using a ptr?
        unordered_map<Chunk*, shared_ptr<Mesh>> m_ChunkMeshes = { };
    };
}
