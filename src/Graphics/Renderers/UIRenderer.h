#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

// TODO: batch the draw calls - receive the sprites, sort them, and construct a single mesh that is drawn in a single call?
// TODO: make an input system that travels down from parent elements to children (or first elements to be added) and "swallows" input events so overlapping elements don't interfere
namespace Minecraft::UIRenderer
{
    inline Camera UICamera;

    void Init();

    void Update();
    void Render();
    void OnResize();

    void DrawSprite(Sprite& sprite);
}
