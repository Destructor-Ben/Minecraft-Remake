#pragma once

#include "Graphics/TextDrawParams.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UIText : public UIElement
    {
    public:
        TextDrawParams Text;

        void SetText(string text);

        virtual void CalculateBounds() override;
        virtual void Render() override;
    };
}
