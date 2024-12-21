#pragma once

#include "BoundingBox.h"

namespace Minecraft
{
    class IndexBuffer;
    class Material;
    class VertexArray;

    class Mesh
    {
    public:
        shared_ptr <VertexArray> Vertices;
        optional <BoundingBox> Bounds;

        Mesh(shared_ptr <VertexArray> vertexArray, optional <BoundingBox> bounds = nullopt) : Vertices(vertexArray), Bounds(bounds) { }

        void AddMaterial(shared_ptr <Material> material, shared_ptr <IndexBuffer> indexBuffer);
        shared_ptr <IndexBuffer> GetIndexBuffer(const shared_ptr <Material>& material);

        void Draw(mat4 transform) const;

    private:
        unordered_map <shared_ptr<Material>, shared_ptr<IndexBuffer>> m_Materials;
    };
}
