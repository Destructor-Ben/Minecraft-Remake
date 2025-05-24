#include "UIElement.h"

#include "Game.h"

namespace Minecraft
{
    Rectangle UIElement::GetParentBounds() const
    {
        if (Parent == nullptr)
            return Instance->ScreenRect;

        return Parent->GetBounds();
    }

    void UIElement::CalculateBounds()
    {
        auto parentBounds = GetParentBounds();

        vec2i position = vec2i(x.Calculate(parentBounds.Width), y.Calculate(parentBounds.Height));
        vec2i size = vec2i(Width.Calculate(parentBounds.Width), Height.Calculate(parentBounds.Height));
        m_Bounds = Rectangle(position, size);

        m_Origin = vec2i(OriginX.Calculate(size.x), OriginY.Calculate(size.y));
    }
}
