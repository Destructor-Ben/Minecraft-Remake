#pragma once

#include "../Common.h"

#include <array>
#include <unordered_map>

#include "Chunk.h"
#include "WorldGenerator.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Camera.h"

namespace Minecraft
{
    // TODO: make some form of static block data
    // TODO: possibly use indexing operator for accessing blocks in world and chunks
    class World
    {
    public:
        Camera Camera;

        // TODO: properly handle world with a vector of Chunks
        Chunk* Chunk;
        class Chunk* Chunk2;
        WorldGenerator* WorldGenerator; // TODO: try to make this not a pointer

        Shader* shader;
        Material* material;
        IndexBuffer* indexBuffer;

        VertexBuffer* vertexBuffer;
        VertexArray* vertexArray;

        Mesh* mesh;

        World();
        ~World();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        // TODO: entity getters
        // TODO: block getters - use indexing operator

    private:
        void UpdateCamera();
    };
}
