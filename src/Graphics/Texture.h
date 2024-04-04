#pragma once

#include "../Common.h"

namespace Minecraft
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        void SetData();

        void Bind() const;

        static void Unbind();

    private:
        uint32 m_ID = 0;
    };
}
