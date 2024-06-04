#include "Mesh.h"

#include "Graphics/IndexBuffer.h"
#include "Graphics/Material.h"
#include "Graphics/VertexArray.h"

namespace Minecraft
{
    int32 Mesh::AddMaterial(shared_ptr<Material> material, shared_ptr<IndexBuffer> indexBuffer)
    {
        m_Materials.push_back({ material, indexBuffer });
        return m_Materials.size() - 1;
    }

    Mesh::MaterialIndicesPair Mesh::GetMaterial(int index)
    {
        return m_Materials.at(index);
    }

    void Mesh::Draw(mat4 transform) const
    {
        Vertices->Bind();

        // TODO: check const-ness of things here
        for (auto [material, indexBuffer] : m_Materials)
        {
            indexBuffer->Bind();
            material->Transform = transform;
            material->Bind();

            glDrawElements(GL_TRIANGLES, (int32)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
}
