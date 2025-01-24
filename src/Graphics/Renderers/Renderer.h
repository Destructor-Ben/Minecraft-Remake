#pragma once

#include "Graphics/CameraFrustum.h"

namespace Minecraft
{
    class Camera;
    class ChunkRenderer;
    class CubeMap;
    class DebugMaterial;
    class FragmentShader;
    class Mesh;
    class Shader;
    class Texture;
    class VertexShader;

    class Renderer
    {
    public:
        Camera* SceneCamera = nullptr;
        bool DrawWireframes = false;

        Renderer();

        void Update();
        void PreRender();
        void PostRender();

        void DrawMesh(const Mesh& mesh, mat4 transform);

        void InitDebugMeshes();
        void DebugDrawPoint(vec3 point, vec3 color);
        void DebugDrawBounds(BoundingBox bounds, vec3 color);

        // Don't forget to free the data after you use it
        byte* LoadImageData(string path, int& width, int& height, int& format);

        shared_ptr <Texture> RequestTexture(string path);
        shared_ptr <CubeMap> RequestCubeMap(string path);
        shared_ptr <Shader> RequestShader(string path);
        shared_ptr <VertexShader> RequestVertexShader(string path);
        shared_ptr <FragmentShader> RequestFragmentShader(string path);

        static void Clear();
        static void UnbindAll();

    private:
        shared_ptr <Mesh> m_DebugPointMesh;
        shared_ptr <Mesh> m_DebugBoundsMesh;
        shared_ptr <DebugMaterial> m_DebugMaterial;

        // Order in this matters!
        vector <string> m_CubeMapFaceNames = {
            "right",
            "left",
            "top",
            "bottom",
            "front",
            "back",
        };

        // Cache resources that are requested multiple times
        unordered_map <string, shared_ptr<Texture>> m_Textures = { };
        unordered_map <string, shared_ptr<CubeMap>> m_CubeMaps = { };
        unordered_map <string, shared_ptr<Shader>> m_Shaders = { };
        unordered_map <string, shared_ptr<VertexShader>> m_VertexShaders = { };
        unordered_map <string, shared_ptr<FragmentShader>> m_FragmentShaders = { };
    };
}
