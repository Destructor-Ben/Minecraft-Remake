#include "UIRenderer.h"

#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/SpriteMaterial.h"
#include "Graphics/Renderers/Renderer.h"
#include "UI/UI.h"
#include "UI/UIState.h"

namespace Minecraft
{
    UIRenderer::UIRenderer()
    {
        InitCamera();
        InitMesh();
    }

    void UIRenderer::InitCamera()
    {
        UICamera.IsPerspective = false;
        UICamera.NearClip = 0;
        UICamera.FarClip = 1;
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
                0, 0,
                1, 0,
                0, 1,
                1, 1,
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
        vertexArray->AddBuffer(vertices);

        m_SpriteMesh = make_shared<Mesh>(vertexArray);
        m_SpriteMesh->Materials[m_SpriteMaterial] = indices;
    }

    void UIRenderer::Update()
    {
        Instance->PerfProfiler->Push("UI::Update");

        UICamera.Update();

        // Update the UI
        for (auto& state : UI::UIStateList)
        {
            state->CheckActive();

            if (!state->Active)
                continue;

            state->Update();
        }

        Instance->PerfProfiler->Pop();
    }

    void UIRenderer::Render()
    {
        Instance->PerfProfiler->Push("UI::Render");

        Instance->Graphics->SceneCamera = &UICamera;

        // Draw the UI
        for (auto& state : UI::UIStateList)
        {
            if (!state->Active)
                continue;

            state->Render();
        }

        Instance->PerfProfiler->Pop();
    }

    void UIRenderer::DrawSprite(Sprite& sprite)
    {
        // Clear the existing depth buffer so it doesn't interfere with the game world
        glClear(GL_DEPTH_BUFFER_BIT);

        m_SpriteMaterial->DrawnSprite = &sprite;
        Instance->Graphics->DrawMesh(*m_SpriteMesh, sprite.GetTransformationMatrix());
    }
}
