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

        void Render();

    private:
        void PrepareSky();
        void PrepareStars();
        void PrepareSkyObjects();

        static shared_ptr <VertexBuffer> CreateQuadVertices();
        static shared_ptr <IndexBuffer> CreateQuadIndices();

        shared_ptr <Mesh> m_SkyMesh;
        shared_ptr <SkyMaterial> m_SkyMaterial;

        int m_StarCount = 0;
        shared_ptr <Mesh> m_StarMesh;
        shared_ptr <StarMaterial> m_StarMaterial;
        shared_ptr <VertexBuffer> m_StarMatricesBuffer;
        shared_ptr <VertexBuffer> m_StarTemperatureBuffer;

        // TODO: rework how sky objects work, maybe make sun and moon use the same shader and stars can have a separate one?
        shared_ptr <Mesh> m_SkyObjectMesh;
        shared_ptr <SkyObjectMaterial> m_SkyObjectMaterial;

        shared_ptr <Texture> m_SunTexture;
        shared_ptr <Texture> m_MoonTexture;
    };
}
