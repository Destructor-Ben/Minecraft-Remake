#include "Mesh.h"

namespace Minecraft
{
    Mesh::Mesh(VertexArray& vertexArray) : Vertices(vertexArray) { }

    // TODO: use references?
    void Mesh::AddMaterial(Material* material, IndexBuffer* indexBuffer)
    {
        Materials.push_back(material);
        Indices.push_back(indexBuffer);
        MaterialCount++;
    }
}
