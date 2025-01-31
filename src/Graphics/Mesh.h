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
        unordered_map <shared_ptr<Material>, shared_ptr<IndexBuffer>> Materials;
        optional <BoundingBox> Bounds;

        Mesh(shared_ptr <VertexArray> vertexArray, optional <BoundingBox> bounds = nullopt) : Vertices(vertexArray), Bounds(bounds) { }

        void Draw(mat4 transform) const;
        void DrawInstanced(mat4 transform, int count) const;
    };
}
