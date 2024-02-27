#include "Renderer.h"

namespace Minecraft
{
    void Renderer::Draw(const VertexArray &vertexArray, const IndexBuffer &indexBuffer, const Shader &shader)
    {
        shader.Bind();
        indexBuffer.Bind();
        vertexArray.Bind();
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
