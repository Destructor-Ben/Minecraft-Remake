#pragma once

#include "Graphics/CameraFrustum.h"

namespace Minecraft
{
    class Camera;
    class ChunkRenderer;
    class DebugMaterial;
    class FragmentShader;
    class Mesh;
    class Shader;
    class Texture;
    class VertexShader;

    // TODO: maybe not here but make a CreateMesh function taking in vertcies, indices, a list of vertex attributes, and a shader
    class Renderer
    {
    public:
        Camera* SceneCamera = nullptr;
        bool DrawWireframes = false;

        Renderer();

        void PreRender();
        void PostRender();

        // TODO: RenderWorld and move all rendering logic out of world class
        void OnEnterWorld();
        void OnExitWorld();

        void DrawMesh(const Mesh& mesh, mat4 transform);

        void InitDebugMeshes();
        void DebugDrawPoint(vec3 point, vec3 color);
        void DebugDrawBounds(const BoundingBox& bounds, mat4 transform, vec3 color);

    private:
        shared_ptr <Mesh> m_DebugPointMesh;
        shared_ptr <Mesh> m_DebugBoundsMesh;
        shared_ptr <DebugMaterial> m_DebugMaterial;
    };
}
