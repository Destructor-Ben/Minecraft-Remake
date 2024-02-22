#pragma once

#include "../Common.h"

namespace Minecraft
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;

        // TODO: push vertex array attributes

        static void Unbind();

    private:
        uint m_ID = 0;
    };
}
