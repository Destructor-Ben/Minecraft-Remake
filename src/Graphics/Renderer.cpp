#include "Renderer.h"

namespace Minecraft
{
    void Renderer::Update()
    {
        if (m_Camera != nullptr)
        {
            ViewMatrix = m_Camera->GetViewMatrix();
            ProjectionMatrix = m_Camera->GetProjectionMatrix();
        }
        else
        {
            ViewMatrix = mat4(1.0f);
            ProjectionMatrix = mat4(1.0f);
        }
    }

    void Renderer::Draw(const Mesh& mesh, const mat4& transform)
    {
        mesh.Vertices.Bind();

        for (int i = 0; i < mesh.MaterialCount; i++)
        {
            IndexBuffer& indexBuffer = *mesh.Indices[i];
            indexBuffer.Bind();

            Material& material = *mesh.Materials[i];
            material.Transform = ProjectionMatrix * ViewMatrix * transform;
            material.Bind();

            glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::UnbindAll()
    {
        VertexArray::Unbind();
        VertexBuffer::Unbind();
        IndexBuffer::Unbind();
        Shader::Unbind();
    }
}
