#include "UIText.h"

#include "Graphics/Renderers/TextRenderer.h"

namespace Minecraft
{
    void UIText::SetText(string text)
    {
        m_Text = text;
        Size = TextRenderer::GetTextSize(text);
    }

    void UIText::Render()
    {
        UIElement::Render();

        // TODO: include origin, scale, and rotation when that is added to the text renderer
        if (HasShadow)
            TextRenderer::DrawTextWithShadow(m_Text, Position - Origin, TextColor, ShadowColor);
        else
            TextRenderer::DrawText(m_Text, Position - Origin, TextColor);
    }
}
