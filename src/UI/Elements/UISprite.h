#pragma once

#include "Graphics/Sprite.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UISprite : public UIElement
    {
    public:
        // Ignore the position and origin, they are overwritten
        Sprite DrawnSprite;

        // Also sets size
        void SetTexture(shared_ptr<Texture> texture);

        virtual void Update() override;
        virtual void Render() override;
    };
}
