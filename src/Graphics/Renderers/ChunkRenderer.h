#pragma once

#include "Hash.h"
#include "World/ChunkPriorityQueue.h"

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

    class ChunkRenderer
    {
    public:
        static constexpr int BlockTextureSize = 16;

        bool DrawChunkBorders = false;

        ChunkRenderer();

        void RenderChunks(const vector<Chunk*>& chunks);
        void RenderChunk(Chunk& chunk);
        void QueueMeshRegen(Chunk& chunk, int priority = 0);

        void RenderDebugChunkBorders();

    private:
        void RegenerateMesh(Chunk& chunk);
        void CreateMesh(Chunk& chunk);
        void SetMeshData(Chunk& chunk, const vector<float>& vertices, const vector <uint>& indices);

        void GetChunkFaces(Chunk& chunk, vector <Quad>& faces);
        void SolidCubeMesh(Block& block, vector <Quad>& faces);
        void GrassPlantMesh(Block& block, vector <Quad>& faces);

        void AddFaceInDirection(Block& block, vector <Quad>& faces, vec3i dir, quat rotation);
        void SetFaceTexture(Quad& face, vec3i dir, vec2i textureCoords);
        // TODO: change the tint property in the vertices to a normal
        // Then calculate the lighting based on the direction to the sun and moon
        // Then also have a global brightness property
        // TODO: probably handle point lights too
        // TODO: maybe the sun and moon should be directional lights
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

        ChunkPriorityQueue m_ChunkRemeshQueue = { };
    };
}
