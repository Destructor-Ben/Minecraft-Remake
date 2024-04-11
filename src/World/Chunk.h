#pragma once

#include "../Common.h"

#include "Block.h"
#include "BlockData.h"
#include "BlockType.h"
#include "Entity.h"
#include "src/Graphics/Mesh.h"
#include "src/Graphics/Texture.h"

namespace Minecraft
{
    // Chunks contain the actual block data and information about it's size, as wel as getters for blocks that access the data
    class Chunk
    {
    public:
        static const uint8 Size = 10;
        static const uint16 Volume = Size * Size * Size;

        struct Face
        {
            vec3 Centre;
            vec3 Normal;
        };

        struct Vertex
        {
            vec3 Position;
            vec2 UV;
        };

        const int32 X;
        const int32 Y;
        const int32 Z;

        Chunk(int32 x, int32 y, int32 z);
        ~Chunk();

        Block GetBlock(uint8 localX, uint8 localY, uint8 localZ);

        // TODO: GetWorldPosition, GetChunkPosition, GetWorldX, GetWorldY, GetWorldZ, make sure similar getters also exist for blocks

        BlockData& GetBlockData(Block block) { return m_BlockData[block.GetID()]; }

        // TODO: make block data components
        /*/
        template<typename T>
        T& GetBlockData(Block block);

        template<typename T>
        T& AddBlockData(Block block);

        template<typename T>
        void RemoveBlockData(Block block);
        //*/

        // TODO: possibly make iterators for entities and blocks in a chunk

        // TODO: entities

        void Tick();
        void Update();
        void Render();

        void RegenerateMesh();

    private:
        void CreateMesh(const std::vector<float>& vertices, const std::vector<uint32>& indices);
        void DeleteMesh();

        std::vector<Face> GetFaces();

        std::array<BlockData, Volume> m_BlockData = std::array<BlockData, Volume>();

        Mesh* m_Mesh = nullptr;
        VertexArray* m_VertexArray = nullptr;
        VertexBuffer* m_VertexBuffer = nullptr;
        IndexBuffer* m_IndexBuffer = nullptr;

        Texture* m_Texture;
        Shader* m_Shader;
        Material* m_Material;

        //std::vector<std::array<BlockType, Volume>> m_BlockData = std::vector<std::array<BlockType, Volume>>();
    };
}
