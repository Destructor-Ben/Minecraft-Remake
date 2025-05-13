#pragma once

#include "Graphics/Sprite.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UISprite : public UIElement
    {
    public:
        // Modify this, not width and height
        // TODO: move all of these fields into here
        Sprite DrawnSprite;

        virtual void Update() override;
        virtual void Render() override;
    };
}
