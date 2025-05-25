#include "UICrosshair.h"

#include "Game.h"
#include "ResourceManager.h"
#include "UI/UI.h"
#include "UI/Elements/UISprite.h"

namespace Minecraft
{
    void UICrosshair::OnInit()
    {
        auto texture = Instance->Resources->RequestTexture("ui/crosshair");
        m_Crosshair = make_shared<UISprite>();
        m_Crosshair->SetTexture(texture, vec2(UI::SpriteScale));
        m_Crosshair->x.Percent = 0.5f;
        m_Crosshair->y.Percent = 0.5f;
        m_Crosshair->OriginX.Percent = 0.5f;
        m_Crosshair->OriginX.Percent = 0.5f;
        AddElement(m_Crosshair);
    }

    void UICrosshair::CheckActive()
    {
        Active = !Instance->IsPaused;

        UIState::CheckActive();
    }
}
