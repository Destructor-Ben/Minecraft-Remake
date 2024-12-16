#pragma once

namespace Minecraft
{
    class Camera;
    class ChunkRenderer;
    class CubeMap;
    class FragmentShader;
    class Mesh;
    class Shader;
    class Texture;
    class VertexShader;

    class Renderer
    {
    public:
        const Camera* Camera = nullptr;
        mat4 ViewMatrix = glm::mat4(1.0f);
        mat4 ProjectionMatrix = glm::mat4(1.0f);

        void Update();

        void DrawMesh(const Mesh& mesh, mat4 transform);

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
        // Order in this matters!
        vector<string> m_CubeMapFaceNames = {
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
