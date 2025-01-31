#include "Mesh.h"

#include "Graphics/IndexBuffer.h"
#include "Graphics/Material.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"

namespace Minecraft
{
    void Mesh::Draw(mat4 transform) const
    {
        Vertices->Bind();

        for (auto [material, indexBuffer] : Materials)
        {
            indexBuffer->Bind();
            material->Transform = transform;
            material->Bind();

            glDrawElements(GL_TRIANGLES, (int)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        IndexBuffer::Unbind();
        Shader::Unbind();
        VertexArray::Unbind();
    }

    void Mesh::DrawInstanced(mat4 transform, int count) const
    {
        Vertices->Bind();

        for (auto [material, indexBuffer] : Materials)
        {
            indexBuffer->Bind();
            material->Transform = transform;
            material->Bind();

            glDrawElementsInstanced(GL_TRIANGLES, (int)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, count);
        }

        IndexBuffer::Unbind();
        Shader::Unbind();
        VertexArray::Unbind();
    }
}
