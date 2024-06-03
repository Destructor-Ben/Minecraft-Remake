#pragma once

#include "../Common.h"

#include "Material.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace Minecraft
{
    class Mesh
    {
    public:
        VertexArray& Vertices;

        // TODO: maybe don't make these public?
        std::vector<Material*> Materials;
        std::vector<IndexBuffer*> Indices;
        int MaterialCount = 0;
        // end TODO

        Mesh(VertexArray& vertexArray);

        void AddMaterial(Material* material, IndexBuffer* indexBuffer);
    };
}
