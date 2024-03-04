#include "Renderer.h"

namespace Minecraft
{
    void Renderer::Draw(const Mesh& mesh, glm::mat4 transform)
    {
        mesh.Vertices.Bind();

        for (int i = 0; i < mesh.MaterialCount; i++)
        {
            Material material = *mesh.Materials[i];
            material.Transform = ProjectionMatrix * ViewMatrix * transform;
            material.Bind();

            IndexBuffer indexBuffer = *mesh.Indices[i];
            indexBuffer.Bind();

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

    glm::mat4 Renderer::CreateOrthographicMatrix(float scale)
    {
        return glm::ortho(0.0f, (float)Window::Width * scale, 0.0f, (float)Window::Height * scale, NearClip, FarClip);
    }

    glm::mat4 Renderer::CreatePerspectiveMatrix(float fov)
    {
        return glm::perspective(fov, (float)Window::Width / (float)Window::Height, NearClip, FarClip);
    }
}
