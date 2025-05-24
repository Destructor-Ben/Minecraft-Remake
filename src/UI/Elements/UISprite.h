#pragma once

#include "Graphics/Sprite.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    // TODO: figure out how this mess will work
    // - Sizing is the problem
    // - Sometimes will want to set the exact dimensions
    // - Sometimes will want to set a scale
    class UISprite : public UIElement
    {
    public:
        // Ignore the position and origin, they are overwritten
        Sprite DrawnSprite;

        // Also sets size
        void SetTexture(shared_ptr <Texture> texture);
        void SetScale(float scale);

        virtual void CalculateBounds() override;
        virtual void Render() override;
    };
}
