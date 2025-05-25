#pragma once

#include "Colors.h"
#include "UI/UIState.h"

namespace Minecraft
{
    class UIText;

    class UIDebug : public UIState
    {
    public:
        virtual void OnInit() override;
        virtual void CheckActive() override;
        virtual void OnUpdate() override;

    private:
        shared_ptr <UIText> MakeText(string text, Color color);

        static constexpr int TextPadding = 10;

        int m_CurrentTextYPos = 0;

        shared_ptr <UIText> m_VSyncText;
        shared_ptr <UIText> m_FrameRateText;
        shared_ptr <UIText> m_TickRateText;

        shared_ptr <UIText> m_PlayerPosText;
        shared_ptr <UIText> m_PlayerChunkText;
        shared_ptr <UIText> m_PlayerBlockPosText;
        shared_ptr <UIText> m_PlayerDirectionText;

        shared_ptr <UIText> m_TargetedBlockPosText;
        shared_ptr <UIText> m_TargetedBlockTypeText;
    };
}
