#pragma once

// TODO: moon phases
// TODO: maybe make the moon travel slightly faster than the sun, especially if I add moon phases
// TODO: finish sky colors
namespace Minecraft
{
    class Mesh;
    class Texture;
    class IndexBuffer;
    class VertexBuffer;

    class SkyMaterial;
    class StarMaterial;
    class SunMoonMaterial;

    class SkyRenderer
    {
    public:
        SkyRenderer();

        void Update();
        void Render();

    private:
        void PrepareSky();
        void PrepareStars();
        void PrepareSunAndMoon();

        void UpdateSkyDarkness(float timePercent);
        void UpdateSunset(float timePercent);

        static shared_ptr<VertexBuffer> CreateQuadVertices();
        static shared_ptr<IndexBuffer> CreateQuadIndices();

        float m_SkyDarkness = 0;
        float m_SunsetStrength = 0;
        vec3 m_SunsetDirection;

        mat4 m_Transform;
        mat4 m_TransformRotated;
        mat4 m_SunTransform;
        mat4 m_MoonTransform;

        shared_ptr<Mesh> m_SkyMesh;
        shared_ptr<SkyMaterial> m_SkyMaterial;

        int m_StarCount = 0;
        shared_ptr<Mesh> m_StarMesh;
        shared_ptr<StarMaterial> m_StarMaterial;
        shared_ptr<VertexBuffer> m_StarMatrixBuffer;
        shared_ptr<VertexBuffer> m_StarBrightnessBuffer;
        shared_ptr<VertexBuffer> m_StarTemperatureBuffer;
        shared_ptr<VertexBuffer> m_StarTwinkleSpeedBuffer;
        shared_ptr<VertexBuffer> m_StarTwinkleOffsetBuffer;
        shared_ptr<VertexBuffer> m_StarTextureIndexBuffer;

        shared_ptr<Mesh> m_SunAndMoonMesh;
        shared_ptr<SunMoonMaterial> m_SunAndMoonMaterial;
    };
}
