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

        void Update();

        void DrawMesh(const Mesh& mesh, const mat4& transform = mat4(1.0f)) const;

        shared_ptr<Texture> RequestTexture(string path, bool mipMap = true);
        shared_ptr<Shader> RequestShader(string path);
        shared_ptr<VertexShader> RequestVertexShader(string path);
        shared_ptr<FragmentShader> RequestFragmentShader(string path);

        static void Clear();
        static void UnbindAll();

        //static std::vector<float> VerticesToFloats(const std::vector<Vertex>& vertices);
        //static (float, float) QuadsToData(const std::vector<Quad>& quads);

    private:
        // Cache resources that are requested multiple times
        unordered_map<string, shared_ptr<Texture>> m_Textures = {};
        unordered_map<string, shared_ptr<Shader>> m_Shaders = {};
        unordered_map<string, shared_ptr<VertexShader>> m_VertexShaders = {};
        unordered_map<string, shared_ptr<FragmentShader>> m_FragmentShaders = {};
    };
}
