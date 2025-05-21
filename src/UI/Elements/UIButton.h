#pragma once

#include "Graphics/TextDrawParams.h"
#include "Graphics/Texture.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UIButton : public UIElement
    {
    public:
        static constexpr int CornerSize = 5;
        static constexpr int EdgeSize = 10;
        static constexpr int Padding = 3;
        static constexpr int Scale = 3;

        std::function<void()> OnMouseDown;
        std::function<void()> OnMouseUp;

        TextDrawParams Text;

        UIButton();

        // Accounts for padding + corner size
        void SetButtonSize(vec2i size);
        void SetButtonSize(int width, int height) { SetButtonSize(vec2i(width, height)); }
        void SetText(string text);

        bool IsHovered() const { return m_IsHovered; }

        virtual void Update() override;
        virtual void Render() override;

    private:
        bool m_IsHovered;
        shared_ptr <Texture> m_Texture;
    };
}
