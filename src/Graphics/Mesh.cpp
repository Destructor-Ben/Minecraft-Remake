#include "Mesh.h"

namespace Minecraft
{
    Mesh::Mesh(VertexArray &vertexArray) : Vertices(vertexArray) { }

    void Mesh::AddMaterial(const Material* material, const IndexBuffer* indexBuffer)
    {
        Materials.push_back(material);
        Indices.push_back(indexBuffer);
        MaterialCount++;
    }
}
