#include "UIState.h"

#include "Game.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    void UIState::CheckActive()
    {
        if (IsInGameUI)
            Active &= Instance->InGame;
        else
            Active &= !Instance->InGame;
    }

    void UIState::Update()
    {
        for (auto& element : m_Elements)
        {
            if (!element->Active)
                continue;

            element->Update();
        }
    }

    void UIState::Render()
    {
        for (auto& element : m_Elements)
        {
            if (!element->Active)
                continue;

            element->Render();
        }
    }

    void UIState::AddElement(shared_ptr <UIElement> element)
    {
        m_Elements.push_back(element);
        element->OnAdd();
        element->CalculateBounds();
    }

    void UIState::RemoveElement(shared_ptr <UIElement> element)
    {
        auto it = std::find(m_Elements.begin(), m_Elements.end(), element);

        if (it == m_Elements.end())
            return;

        element->OnRemove();
        m_Elements.erase(it);
    }

    void UIState::OnResize()
    {
        for (auto& element : m_Elements)
        {
            element->CalculateBounds();
        }
    }
}
