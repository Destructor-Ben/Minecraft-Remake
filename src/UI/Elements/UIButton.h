#pragma once

#include "Graphics/Texture.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    // TODO: finish button and main menu UI
    class UIButton : public UIElement
    {
    public:
        static constexpr int CornerSize = 5;
        static constexpr int EdgeSize = 10;
        static constexpr int Padding = 3;

        std::function<void()> OnMouseDown;
        std::function<void()> OnMouseUp;

        UIButton();

        // Accounts for padding + corner size
        void SetButtonSize(vec2i size);

        bool IsHovered() const { return m_IsHovered; }

        virtual void Update() override;
        virtual void Render() override;

    private:
        bool m_IsHovered;
        shared_ptr <Texture> m_Texture;
    };
}
