#pragma once

#include "../Common.h"

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
        void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);

        static void Clear();
        static void UnbindAll();
    };
}
