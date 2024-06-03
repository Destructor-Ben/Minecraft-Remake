#pragma once

#include "../Common.h"

namespace Minecraft
{
    class VertexShader
    {
    public:
        explicit VertexShader(const string& source);
        ~VertexShader();

        uint32 GetID() const;

    private:
        uint32 m_ID = 0;
    };
}
