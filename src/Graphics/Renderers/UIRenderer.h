#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

// TODO: implement UI scale
// TODO: batch the draw calls - receive the sprites, sort them, and construct a single mesh that is drawn in a single call?
namespace Minecraft
{
    class Mesh;
    class SpriteMaterial;

    class UIRenderer
    {
    public:
        Camera UICamera;

        UIRenderer();

        void DrawSprite(Sprite& sprite);

    private:
        void InitCamera();
        void InitMesh();

        shared_ptr <Mesh> m_SpriteMesh;
        shared_ptr <SpriteMaterial> m_SpriteMaterial;
    };
}
