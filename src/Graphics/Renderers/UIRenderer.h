#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

// TODO: implement UI scale
// TODO: batch the draw calls - receive the sprites, sort them, and construct a single mesh that is drawn in a single call?
// TODO: make a begin and end function, as well as for renderer instead of PreRender and PostRender, also do wireframes for UI there
// TODO: UI system
namespace Minecraft
{
    class Mesh;
    class SpriteMaterial;

    class UIRenderer
    {
    public:
        Camera UICamera;

        UIRenderer();

        void Render();

        void DrawSprite(Sprite& sprite);

    private:
        void InitCamera();
        void InitMesh();

        shared_ptr <Mesh> m_SpriteMesh;
        shared_ptr <SpriteMaterial> m_SpriteMaterial;
    };
}
