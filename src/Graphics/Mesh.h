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
        std::vector<const Material*> Materials;
        std::vector<const IndexBuffer*> Indices;
        int MaterialCount = 0;
        // end TODO

        Mesh(VertexArray& vertexArray);

        void AddMaterial(const Material* material, const IndexBuffer* indexBuffer);
    };
}
