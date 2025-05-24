#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

// TODO: implement UI scale
// TODO: batch the draw calls - receive the sprites, sort them, and construct a single mesh that is drawn in a single call?
// TODO: make a begin and end function, as well as for renderer instead of PreRender and PostRender, also do wireframes for UI there
// TODO: make an input system that travels down from parent elements to children (or first elements to be added) and "swallows" input events so overlapping elements don't interfere
namespace Minecraft
{
    class Mesh;
    class SpriteMaterial;
    class UIState;

    class UIRenderer
    {
    public:
        Camera UICamera;

        UIRenderer();

        void Update();
        void Render();
        void OnResize();

        void DrawSprite(Sprite& sprite);

    private:
        void InitCamera();
        void InitMesh();

        shared_ptr <Mesh> m_SpriteMesh;
        shared_ptr <SpriteMaterial> m_SpriteMaterial;
    };
}
