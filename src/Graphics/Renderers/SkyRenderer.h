#pragma once

namespace Minecraft
{
    class Mesh;
    class SkyMaterial;
    class SkyObjectMaterial;
    class Texture;

    class SkyRenderer
    {
    public:
        SkyRenderer();

        void Render();

    private:
        void PrepareSky();
        void PrepareSkyObjects();

        vector<vec3> m_StarPositions;

        shared_ptr <Mesh> m_SkyMesh;
        shared_ptr <SkyMaterial> m_SkyMaterial;

        shared_ptr <Mesh> m_SkyObjectMesh;
        shared_ptr <SkyObjectMaterial> m_SkyObjectMaterial;

        shared_ptr <Texture> m_SunTexture;
        shared_ptr <Texture> m_MoonTexture;
        shared_ptr <Texture> m_StarTexture;
    };
}
