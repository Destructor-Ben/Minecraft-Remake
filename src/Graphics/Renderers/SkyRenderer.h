#pragma once

#include "Graphics/Renderers/SkyMaterial.h"

// TODO: make the sky more minecraft with a gradient
// TODO: sun and moon
// TODO: stars
// Time can probably be done by just rotating the cube? idk, color does change too
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
