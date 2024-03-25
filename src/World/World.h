#pragma once

#include "../Common.h"

#include <map>

#include "Chunk.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Camera.h"

namespace Minecraft
{
    class World
    {
    public:
        Camera Camera;

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
        // TODO: block getters

    private:
        void UpdateCamera();
    };
}
