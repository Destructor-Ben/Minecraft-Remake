#pragma once

#include "Common.h"

// If we wanted to, we could make the graphics framework more bulletproof and add more abstractions for things like enums and checks if methods have already been called, but I'm lazy

namespace Minecraft
{
    class Camera;
    class ChunkRenderer;
    class FragmentShader;
    class Mesh;
    class Shader;
    class Texture;
    class VertexShader;

    class Renderer
    {
    public:
        shared_ptr<ChunkRenderer> ChunkRenderer;

        const Camera* Camera = nullptr;
        mat4 ViewMatrix = glm::mat4(1.0f);
        mat4 ProjectionMatrix = glm::mat4(1.0f);

        Renderer();

        void Update();

        void DrawMesh(const Mesh& mesh, mat4 transform);

        shared_ptr<Texture> RequestTexture(string path);
        shared_ptr<Shader> RequestShader(string path);
        shared_ptr<VertexShader> RequestVertexShader(string path);
        shared_ptr<FragmentShader> RequestFragmentShader(string path);

        static void Clear();
        static void UnbindAll();

        // TODO: implement these
        //static vector<float32> VerticesToFloats(const vector<Vertex>& vertices);
        //static (float32, float32) QuadsToData(const vector<Quad>& quads);

    private:
        // Cache resources that are requested multiple times
        unordered_map<string, shared_ptr<Texture>> m_Textures = { };
        unordered_map<string, shared_ptr<Shader>> m_Shaders = { };
        unordered_map<string, shared_ptr<VertexShader>> m_VertexShaders = { };
        unordered_map<string, shared_ptr<FragmentShader>> m_FragmentShaders = { };
    };
}
