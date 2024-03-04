#pragma once

#include "../Common.h"

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

        // TODO: make these parameters in the matrix functions
        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        void Draw(const Mesh& mesh, glm::mat4 transform);

        // TODO: perspective doesn't remap -1:1 to 0:Width/Height
        glm::mat4 CreateOrthographicMatrix(float scale = 1.0f);
        glm::mat4 CreatePerspectiveMatrix(float fov = 45.0f);

        static void Clear();
        static void UnbindAll();
    };
}
