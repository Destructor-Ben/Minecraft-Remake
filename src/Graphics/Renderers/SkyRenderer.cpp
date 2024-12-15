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
        mat4 view = Instance->Graphics->ProjectionMatrix;
        view = mat4(mat3(view)); // Remove translation
        mat4 transform = projection * view;

        // Don't use Renderer.Draw, it is for normal objects
        //m_SkyMesh->Draw(transform);
    }

    void SkyRenderer::CreateMesh()
    {
        /*/ Create the material
        auto shader = Instance->Graphics->RequestShader("shader");
        m_SkyMaterial = make_shared<SkyMaterial>(shader);

        // Create the vertex and index buffers
        auto vertexBuffer = make_shared<VertexBuffer>();
        auto indexBuffer = make_shared<IndexBuffer>();

        // Set the mesh data
        vertexBuffer->SetData(
            {
                0.0, 0.0, 0.0,
                0.0, -1.0, 0.0,
                1.0, 0.0, 0.0,
            }
        );

        indexBuffer->SetData(
            {
                0, 1, 2,
            }
        );

        // Create the vertex array
        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(3);
        vertexArray->AddBuffer(vertexBuffer);

        // Create the mesh
        auto mesh = make_shared<Mesh>(vertexArray);
        mesh->AddMaterial(m_SkyMaterial, indexBuffer);//*/
    }
}
