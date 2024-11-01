#pragma once

namespace Minecraft
{
    class Material;
    class IndexBuffer;
    class VertexArray;

    class Mesh
    {
    public:
        shared_ptr<VertexArray> Vertices;

        Mesh(shared_ptr<VertexArray> vertexArray) : Vertices(vertexArray) { }

        void AddMaterial(shared_ptr<Material> material, shared_ptr<IndexBuffer> indexBuffer);
        shared_ptr<IndexBuffer> GetIndexBuffer(const shared_ptr<Material>& material);

        void Draw(mat4 transform) const;

    private:
        unordered_map<shared_ptr<Material>, shared_ptr<IndexBuffer>> m_Materials;
    };
}
