#include "SkyRenderer.h"

#include "Game.h"
#include "Graphics/GL.h"
#include "Graphics/Renderers/Renderer.h"

namespace Minecraft
{
    SkyRenderer::SkyRenderer()
    {
        CreateMesh();
    }

    void SkyRenderer::Render()
    {
        mat4 projection = Instance->Graphics->ProjectionMatrix;
        mat4 view = Instance->Graphics->ViewMatrix;
        view = mat4(mat3(view)); // Remove translation
        mat4 transform = projection * view;

        // Since we draw after the scene, we use a trick to make sure the depth value is always 1
        // This means we need to change the depth function though because otherwise we won't be able to actually write to the pixels
        // But we still need depth testing
        // Also whether we write to the depth mask doesn't matter, since we would be overwriting 1 (existing value) with 1 (new value)
        glDepthFunc(GL_LEQUAL);

        // Don't use Renderer.Draw, it is for normal objects
        m_SkyMesh->Draw(transform);

        glDepthFunc(GL_LESS);
    }

    void SkyRenderer::CreateMesh()
    {
        // Create the material
        auto shader = Instance->Graphics->RequestShader("sky");
        auto cubeMap = Instance->Graphics->RequestCubeMap("sky");
        m_SkyMaterial = make_shared<SkyMaterial>(shader);
        m_SkyMaterial->Texture = cubeMap;

        // Create the vertex and index buffers
        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        // Set the mesh data
        // ChatGPT made this for me, and I think the z value of every vertex is negated
        vertexBuffer->SetData(
            {
                // Front face (positive Z)
                -1.0f, 1.0f, 1.0f,  // 0: top-left
                -1.0f, -1.0f, 1.0f, // 1: bottom-left
                1.0f, -1.0f, 1.0f,  // 2: bottom-right
                1.0f, 1.0f, 1.0f,   // 3: top-right

                // Back face (negative Z)
                -1.0f, 1.0f, -1.0f,  // 4: top-left
                -1.0f, -1.0f, -1.0f, // 5: bottom-left
                1.0f, -1.0f, -1.0f,  // 6: bottom-right
                1.0f, 1.0f, -1.0f,   // 7: top-right
            }
        );

        indexBuffer->SetData(
            {
                // Front face
                2, 1, 0,
                3, 2, 0,

                // Back face
                4, 5, 6,
                4, 6, 7,

                // Left face
                1, 5, 4,
                0, 1, 4,

                // Right face
                6, 2, 3,
                7, 6, 3,

                // Top face
                3, 0, 4,
                7, 3, 4,

                // Bottom face
                6, 5, 1,
                2, 6, 1,
            }
        );

        // Create the vertex array
        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->AddBuffer(vertexBuffer);

        // Create the mesh
        m_SkyMesh = make_shared<Mesh>(vertexArray);
        m_SkyMesh->AddMaterial(m_SkyMaterial, indexBuffer);
    }
}
