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
        static constexpr int BlockTextureSize = 16;

        bool DrawChunkBorders = false;

        ChunkRenderer();

        void RenderChunks(const vector<Chunk*>& chunks);
        void RenderChunk(Chunk& chunk);
        void RegenerateMesh(Chunk& chunk);

        void RenderDebugChunkBorders();

    private:
        void CreateMesh(Chunk& chunk);
        void SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices);

        vector <Quad> GetChunkFaces(Chunk& chunk);
        void SolidCubeMesh(Block& block, vector <Quad>& faces);
        void GrassPlantMesh(Block& block, vector <Quad>& faces);

        void AddFaceInDirection(Block& block, vector <Quad>& faces, vec3i dir, quat rotation);
        void SetFaceTexture(Quad& face, vec3i dir, vec2i textureCoords);
        vec3 GetFaceTint(vec3i dir);

        vec2 m_BlockTextureSizeInUVCoords;

        static constexpr float Degrees180 = (float)numbers::pi;
        static constexpr float Degrees90 = (float)numbers::pi / 2.0f;
        static constexpr float Degrees45 = (float)numbers::pi / 4.0f;

        array<quat, 6> m_BlockFaceDirections = {
            // Top and bottom
            glm::identity<quat>(),
            glm::eulerAngleX(Degrees180) * glm::eulerAngleY(Degrees180),

            // Left and right
            glm::eulerAngleX(Degrees90) * glm::eulerAngleZ(-Degrees90),
            glm::eulerAngleX(Degrees90) * glm::eulerAngleZ(Degrees90),

            // Front and back
            glm::eulerAngleX(Degrees90),
            glm::eulerAngleX(Degrees90) * glm::eulerAngleZ(Degrees180),
        };

        quat m_GrassPlantFaceRotation = glm::eulerAngleX(Degrees90) * glm::eulerAngleZ(Degrees45);

        shared_ptr <Texture> m_ChunkTexture;
        shared_ptr <ChunkMaterial> m_ChunkMaterial;

        unordered_map <vec3i, shared_ptr<Mesh>> m_ChunkMeshes = { };
        unordered_map<vec3i, bool> m_IsChunkMeshEmpty = { };
    };
}
