#pragma once

#include "Graphics/Renderers/SkyMaterial.h"

namespace Minecraft
{
    class Mesh;

    class SkyRenderer
    {
    public:
        SkyRenderer();

        void Render();

    private:
        void CreateMesh();

        shared_ptr <Mesh> m_SkyMesh;
        shared_ptr <SkyMaterial> m_SkyMaterial;
    };
}
