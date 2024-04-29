#pragma once

#include "../Common.h"

#include "Camera.h"
#include "Mesh.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Window.h"
#include "ChunkRenderer.h"
#include "GraphicsResource.h"

namespace Minecraft
{
    class Renderer
    {
    public:
        ChunkRenderer* ChunkRenderer;

        const Camera* Camera = nullptr;
        glm::mat4 ViewMatrix = glm::mat4(1.0f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

        ~Renderer();

        void Update();

        void DrawMesh(const Mesh& mesh, const mat4& transform = mat4(1.0f)) const;

        // TODO: embed resources (assets, lang, and data) into executable by creating a cpp and header file for each resources with their info (path, size, and bytes) - https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmakexture& RequestTexture(string path);
        void TrackGraphicsResource(GraphicsResource* resource);
        Texture& RequestTexture(string path, bool hasAlpha = false, bool mipMap = true);
        Shader& RequestShader(string path);
        VertexShader& RequestVertexShader(string path);
        FragmentShader& RequestFragmentShader(string path);

        static void Clear();
        static void UnbindAll();

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
