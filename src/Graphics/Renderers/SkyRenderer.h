#pragma once

// TODO: make the sky more minecraft with a gradient
// TODO: sun and moon
// TODO: stars
// Time can probably be done by just rotating the cube
// Also by changing the sky color based on the time
namespace Minecraft
{
    class Mesh;
    class SkyMaterial;

    class SkyRenderer
    {
    public:
        SkyRenderer();

        void Render();

    private:
        void Prepare();

        shared_ptr<Mesh> m_SkyMesh;
        shared_ptr<SkyMaterial> m_SkyMaterial;

        // TODO: implement these
        shared_ptr<Mesh> m_QuadMesh;
        shared_ptr<SkyMaterial> m_SunMaterial;
        shared_ptr<SkyMaterial> m_MoonMaterial;
        shared_ptr<SkyMaterial> m_StarMaterial;
    };
}
