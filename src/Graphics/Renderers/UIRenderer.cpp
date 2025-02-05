#include "UIRenderer.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/SpriteMaterial.h"
#include "Graphics/Renderers/Renderer.h"

namespace Minecraft
{// TODO: make a begin and end function, as well as for renderer instead of PreRender and PostRender, also do wireframes for UI there
    UIRenderer::UIRenderer()
    {
        InitCamera();
        InitMesh();
    }

    void UIRenderer::InitCamera()
    {
        UICamera.IsPerspective = false;
        // TODO: maybe make the near clip plane 0
        UICamera.NearClip = 0.1f;
        UICamera.FarClip = 1.0f;

        // Changing this changes the UI scale
        UICamera.OrthographicScale = 1.0f;

        UICamera.Update();
    }

    void UIRenderer::InitMesh()
    {
        // Create the material
        auto shader = Instance->Resources->RequestShader("sprite");
        m_SpriteMaterial = make_shared<SpriteMaterial>(shader);

        // Create the mesh
        auto vertices = make_shared<VertexBuffer>();
        vertices->SetData(vector<float>(
            {
                -0.5f, -0.5f,
                0, 1,
                0.5f, -0.5f,
                1, 1,
                -0.5f, 0.5f,
                0, 0,
                0.5f, 0.5,
                1, 0,
            }
        ));

        auto indices = make_shared<IndexBuffer>();
        indices->SetData(vector<uint>(
            {
                0, 1, 2,
                3, 2, 1,
            }
        ));

        auto vertexArray = make_shared<VertexArray>();
        vertexArray->PushFloat(2);
        vertexArray->PushFloat(2);
        vertexArray->AddBuffer(vertices);

        m_SpriteMesh = make_shared<Mesh>(vertexArray);
        m_SpriteMesh->Materials[m_SpriteMaterial] = indices;
    }

    void UIRenderer::DrawSprite(Sprite& sprite)
    {
        // TODO: figure out how to do depth testing without interfering with the game world
        glDisable(GL_DEPTH_TEST);
        m_SpriteMaterial->DrawnSprite = &sprite;
        Instance->Graphics->DrawMesh(*m_SpriteMesh, sprite.GetTransformationMatrix());
        glEnable(GL_DEPTH_TEST);
    }
}
