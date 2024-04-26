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

namespace Minecraft
{
    class Renderer
    {
    public:
        glm::mat4 ViewMatrix = glm::mat4(1.0f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

        ~Renderer();

        void SetCamera(const Camera* camera) { m_Camera = camera; }

        void Update();

        void DrawMesh(const Mesh& mesh, const mat4& transform = mat4(1.0f));

        // TODO: embed resources (assets, lang, and data) into executable by creating a cpp and header file for each resources with their info (path, size, and bytes) - https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmakexture& RequestTexture(string path);
        Texture& RequestTexture(string path, int32 format = -1, bool mipMap = true);
        Shader& RequestShader(string path);
        VertexShader& RequestVertexShader(string path);
        FragmentShader& RequestFragmentShader(string path);

        static void Clear();
        static void UnbindAll();

    private:
        template<typename T>
        void DeleteResources(const std::unordered_map<string, T*>& resources);

        const Camera* m_Camera = nullptr;

        // TODO: make these pointers references instead, and have a vector of pointers for graphics resources that can be automatically deallocated, with these here only for caching the requests
        std::unordered_map<string, Texture*> m_Textures = {};
        std::unordered_map<string, Shader*> m_Shaders = {};
        std::unordered_map<string, VertexShader*> m_VertexShaders = {};
        std::unordered_map<string, FragmentShader*> m_FragmentShaders = {};
    };
}
