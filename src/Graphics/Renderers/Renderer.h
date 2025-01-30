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
        void DebugDrawBounds(const BoundingBox& bounds, mat4 transform, vec3 color);

        // TODO: these functions are useless, once binding is properly sorted, remove unbindall
        static void UnbindAll();

    private:
        shared_ptr <Mesh> m_DebugPointMesh;
        shared_ptr <Mesh> m_DebugBoundsMesh;
        shared_ptr <DebugMaterial> m_DebugMaterial;
    };
}
