#pragma once

#include "Graphics/Sprite.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UISprite : public UIElement
    {
    public:
        // Ignore position, origin, and size, since they are overwritten
        Sprite DrawnSprite;

        void SetTexture(shared_ptr <Texture> texture, vec2 scale = vec2(1));

        virtual void RecalculateBounds() override;
        virtual void OnRender() override;
    };
}
