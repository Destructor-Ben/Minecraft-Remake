#pragma once

#include "Graphics/TextDrawParams.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UIText : public UIElement
    {
    public:
        TextDrawParams Text;

        void SetText(string text, vec2 scale = vec2(1));

        virtual void RecalculateBounds() override;
        virtual void OnRender() override;
    };
}
