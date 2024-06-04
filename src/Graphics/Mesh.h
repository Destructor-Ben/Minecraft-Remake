#pragma once

#include "Common.h"

namespace Minecraft
{
    class Material;
    class IndexBuffer;
    class VertexArray;

    class Mesh
    {
    public:
        struct MaterialIndicesPair
        {
            shared_ptr<Material> Material;
            shared_ptr<IndexBuffer> Indices;
        };

        shared_ptr<VertexArray> Vertices;

        Mesh(shared_ptr<VertexArray> vertexArray) : Vertices(vertexArray) { }

        // Returns the index of the material
        // TODO: make a more elegant way of getting materials once they have been added - make the material a key in a hashtable?
        int32 AddMaterial(shared_ptr<Material> material, shared_ptr<IndexBuffer> indexBuffer);
        MaterialIndicesPair GetMaterial(int32 index);

        void Draw(mat4 transform) const;

    private:
        vector<MaterialIndicesPair> m_Materials;
    };
}
