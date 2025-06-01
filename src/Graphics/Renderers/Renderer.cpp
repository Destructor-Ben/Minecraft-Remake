#include "Renderer.h"

#include "Game.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "Graphics/GL.h"
#include "Graphics/Materials/DebugMaterial.h"
#include "Graphics/Renderers/ChunkRenderer.h"

namespace Minecraft
{
    Renderer::Renderer()
    {
        InitDebugMeshes();
    }

    void Renderer::PreRender()
    {
        if (DrawWireframes)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Renderer::PostRender()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // TODO: allow toggling frustum culling
    void Renderer::DrawMesh(const Mesh& mesh, mat4 transform)
    {
        // Frustum culling
        if (mesh.Bounds.has_value())
        {
            // Transform the bounds
            auto bounds = mesh.Bounds.value().Transformed(transform);

            // Check if the frustum contains the bounds
            if (!SceneCamera->Frustum.ContainsBounds(bounds))
                return;
        }

        mesh.Draw(SceneCamera->ProjectionViewMatrix * transform);
    }

    void Renderer::InitDebugMeshes()
    {
        // Create debug material
        auto shader = Resources::RequestShader("debug");
        m_DebugMaterial = make_shared<DebugMaterial>(shader);

        // Create point mesh
        float pointVertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };

        auto pointVertexBuffer = make_shared<VertexBuffer>();
        pointVertexBuffer->SetData(pointVertices, 4 * 3);

        uint pointIndices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        auto pointIndexBuffer = make_shared<IndexBuffer>();
        pointIndexBuffer->SetData(pointIndices, 6);

        auto pointVertexArray = make_shared<VertexArray>();
        pointVertexArray->PushFloat(3);
        pointVertexArray->AddBuffer(pointVertexBuffer);

        m_DebugPointMesh = make_shared<Mesh>(pointVertexArray);
        m_DebugPointMesh->Materials[m_DebugMaterial] = pointIndexBuffer;

        // Create bounds mesh
        float boundsVertices[] = {
            0.0f, 0.0f, 0.0f, // 0
            1.0f, 0.0f, 0.0f, // 1
            1.0f, 1.0f, 0.0f, // 2
            0.0f, 1.0f, 0.0f, // 3
            0.0f, 0.0f, 1.0f, // 4
            1.0f, 0.0f, 1.0f, // 5
            1.0f, 1.0f, 1.0f, // 6
            0.0f, 1.0f, 1.0f, // 7
        };

        auto boundsVertexBuffer = make_shared<VertexBuffer>();
        boundsVertexBuffer->SetData(boundsVertices, 8 * 3);

        uint boundsIndices[] = {
            // Front face
            0, 2, 1,
            0, 3, 2,
            // Back face
            4, 5, 6,
            4, 6, 7,
            // Left face
            0, 7, 3,
            0, 4, 7,
            // Right face
            1, 6, 5,
            1, 2, 6,
            // Top face
            3, 6, 2,
            3, 7, 6,
            // Bottom face
            0, 5, 4,
            0, 1, 5,
        };

        auto boundsIndexBuffer = make_shared<IndexBuffer>();
        boundsIndexBuffer->SetData(boundsIndices, 6 * 2 * 3);

        auto boundsVertexArray = make_shared<VertexArray>();
        boundsVertexArray->PushFloat(3);
        boundsVertexArray->AddBuffer(boundsVertexBuffer);

        m_DebugBoundsMesh = make_shared<Mesh>(boundsVertexArray);
        m_DebugBoundsMesh->Materials[m_DebugMaterial] = boundsIndexBuffer;
    }

    void Renderer::DebugDrawPoint(vec3 point, vec3 color)
    {
        // Move to the position and face the camera
        auto transform = glm::translate(point);
        transform *= toMat4(glm::quatLookAt(SceneCamera->GetForwardVector(), vec3(0, 1, 0)));

        m_DebugMaterial->Color = color;
        m_DebugMaterial->DrawCircle = true;
        DrawMesh(*m_DebugPointMesh, transform);
    }

    void Renderer::DebugDrawBounds(const BoundingBox& bounds, mat4 transform, vec3 color)
    {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        auto transformedBounds = bounds.Transformed(transform);
        auto meshTransform = glm::translate(transformedBounds.Min);
        meshTransform *= glm::scale(transformedBounds.GetSize());

        m_DebugMaterial->Color = color;
        m_DebugMaterial->DrawCircle = false;
        DrawMesh(*m_DebugBoundsMesh, meshTransform);

        glEnable(GL_CULL_FACE);
        if (!DrawWireframes)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
