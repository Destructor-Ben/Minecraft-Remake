#pragma once

namespace Minecraft
{
    class Mesh;
    class Texture;
    class IndexBuffer;
    class VertexBuffer;
    class SkyMaterial;
    class SkyObjectMaterial;
    class StarMaterial;

    class SkyRenderer
    {
    public:
        SkyRenderer();

        void Update();
        void Render();

    private:
        void PrepareSky();
        void PrepareStars();
        void PrepareSkyObjects();

        void UpdateSkyDarkness(float timePercent);
        void UpdateSunsetStrength(float timePercent);

        static shared_ptr <VertexBuffer> CreateQuadVertices();
        static shared_ptr <IndexBuffer> CreateQuadIndices();

        float m_SkyDarkness = 0;
        float m_SunsetStrength = 0;
        mat4 m_Transform;
        mat4 m_TransformRotated;

        shared_ptr <Mesh> m_SkyMesh;
        shared_ptr <SkyMaterial> m_SkyMaterial;

        int m_StarCount = 0;
        shared_ptr <Mesh> m_StarMesh;
        shared_ptr <StarMaterial> m_StarMaterial;
        shared_ptr <VertexBuffer> m_StarMatrixBuffer;
        shared_ptr <VertexBuffer> m_StarBrightnessBuffer;
        shared_ptr <VertexBuffer> m_StarTemperatureBuffer;

        // TODO: rework how sky objects work, maybe make sun and moon use the same shader and stars can have a separate one?
        shared_ptr <Mesh> m_SkyObjectMesh;
        shared_ptr <SkyObjectMaterial> m_SkyObjectMaterial;

        shared_ptr <Texture> m_SunTexture;
        shared_ptr <Texture> m_MoonTexture;
    };
}
