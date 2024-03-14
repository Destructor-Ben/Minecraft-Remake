#pragma once

#include "../Common.h"

#include "Camera.h"
#include "Mesh.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Window.h"

namespace Minecraft
{
    class Renderer
    {
    public:
        glm::mat4 ViewMatrix = glm::mat4(1.0f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

        void SetCamera(const Camera* camera) { m_Camera = camera; }

        void Update();

        void Draw(const Mesh& mesh, const glm::mat4& transform);

        static void Clear();
        static void UnbindAll();

    private:
        const Camera* m_Camera;
    };
}
