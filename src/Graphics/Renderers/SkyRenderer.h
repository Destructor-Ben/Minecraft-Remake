#pragma once

#include "Config.h"
#include "World/State/SkyState.h"

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

        void Update(SkyState &sky);
        void Render();

    private:
        void PrepareSky();
        void PrepareStars();
        void PrepareSunAndMoon();

        void UpdateSkyDarkness(float timePercent);
        void UpdateSunset(float timePercent);

        static shared_ptr<VertexBuffer> CreateQuadVertices();
        static shared_ptr<IndexBuffer> CreateQuadIndices();

        Config m_Config;

        // TODO: move these to a SkyRendererState
        mat4 m_Transform;
        mat4 m_TransformRotated;
        mat4 m_SunTransform;
        mat4 m_MoonTransform;

        shared_ptr<Mesh> m_SkyMesh;
        shared_ptr<SkyMaterial> m_SkyMaterial;

        int m_StarCount = 0;
        shared_ptr<Mesh> m_StarMesh;
        shared_ptr<StarMaterial> m_StarMaterial;

        shared_ptr<Mesh> m_SunAndMoonMesh;
        shared_ptr<SunMoonMaterial> m_SunAndMoonMaterial;
    };
}
