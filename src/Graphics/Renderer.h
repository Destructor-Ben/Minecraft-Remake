#pragma once

#include "../Common.h"

#include "Camera.h"
#include "Mesh.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ChunkRenderer.h"
#include "GraphicsResource.h"
#include "Vertex.h"

namespace Minecraft
{
    class Renderer
    {
    public:
        shared_ptr<ChunkRenderer> ChunkRenderer;

        const Camera* Camera = nullptr;
        glm::mat4 ViewMatrix = glm::mat4(1.0f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

        Renderer();
        ~Renderer();

        void Update();

        void DrawMesh(const Mesh& mesh, const mat4& transform = mat4(1.0f)) const;

        void TrackGraphicsResource(GraphicsResource* resource);
        Texture& RequestTexture(string path, bool hasAlpha = false, bool mipMap = true);
        Shader& RequestShader(string path);
        VertexShader& RequestVertexShader(string path);
        FragmentShader& RequestFragmentShader(string path);

        static void Clear();
        static void UnbindAll();

        //static std::vector<float> VerticesToFloats(const std::vector<Vertex>& vertices);
        //static (float, float) QuadsToData(const std::vector<Quad>& quads);

    private:
        // For owning the pointers for all graphics resources
        std::vector<GraphicsResource*> m_GraphicsResources = {};
        // For caching resources that are requested multiple times
        std::unordered_map<string, Texture*> m_Textures = {};
        std::unordered_map<string, Shader*> m_Shaders = {};
        std::unordered_map<string, VertexShader*> m_VertexShaders = {};
        std::unordered_map<string, FragmentShader*> m_FragmentShaders = {};
    };
}
