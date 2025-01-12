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
        struct Star
        {
            vec3 Position = vec3(0);
            float Rotation = 0;
            float Scale = 1;
            float Temperature = 0; // TODO: implement temperature
        };

        void PrepareSky();
        void PrepareSkyObjects();
        void PrepareStars();

        vector<Star> m_Stars;

        shared_ptr<Mesh> m_SkyMesh;
        shared_ptr<SkyMaterial> m_SkyMaterial;

        // TODO: rework how sky objects work, maybe make sun and moon use the same shader and stars can have a separate one?
        shared_ptr<Mesh> m_SkyObjectMesh;
        shared_ptr<SkyObjectMaterial> m_SkyObjectMaterial;

        shared_ptr<Texture> m_SunTexture;
        shared_ptr<Texture> m_MoonTexture;
        shared_ptr<Texture> m_StarTexture;
    };
}
