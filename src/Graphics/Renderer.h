#pragma once

#include "../Common.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Window.h"

// TODO: temporary
class Mesh;

namespace Minecraft
{
    class Renderer
    {
    public:
        void Draw(const Mesh& mesh);

        static void Clear();
        static void UnbindAll();
    };
}
