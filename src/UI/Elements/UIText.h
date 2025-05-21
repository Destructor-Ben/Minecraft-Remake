#pragma once

#include "UI/UIElement.h"

namespace Minecraft
{
    class UIText : public UIElement
    {
    public:
        vec3 TextColor = vec3(1);
        optional <vec3> ShadowColor = nullopt;
        bool HasShadow = true;

        void SetText(string text);

        virtual void Render() override;

    private:
        string m_Text;
    };
}
