#include "UIElement.h"

#include "Game.h"

namespace Minecraft
{
    void UIElement::AddElement(shared_ptr <UIElement> element)
    {
        m_Children.push_back(element);
        element->m_Parent = this;

        element->OnAdd();
        element->RecalculateBounds();
    }

    void UIElement::RemoveElement(shared_ptr <UIElement> element)
    {
        auto it = std::find(m_Children.begin(), m_Children.end(), element);
        if (it == m_Children.end())
            return;

        element->OnRemove();
        element->m_Parent = nullptr;
        m_Children.erase(it);
    }

    void UIElement::Update()
    {
        OnUpdate();

        for (auto& child : m_Children)
        {
            if (!child->Active)
                continue;

            child->Update();
        }
    }

    void UIElement::Render()
    {
        OnRender();

        for (auto& child : m_Children)
        {
            if (!child->Active)
                continue;

            child->Render();
        }
    }

    void UIElement::ScreenResized()
    {
        OnResize();

        for (auto& child : m_Children)
        {
            if (!child->Active)
                continue;

            child->ScreenResized();
        }
    }

    void UIElement::RecalculateBounds()
    {
        auto parentBounds = GetParentBounds();

        vec2i position = vec2i(x.Calculate(parentBounds.Width), y.Calculate(parentBounds.Height));
        vec2i size = vec2i(Width.Calculate(parentBounds.Width), Height.Calculate(parentBounds.Height));
        m_Bounds = Rectangle(position, size);
        m_Origin = vec2i(OriginX.Calculate(size.x), OriginY.Calculate(size.y));

        // Recalculate children
        for (auto& child : m_Children)
        {
            if (!child->Active)
                continue;

            child->RecalculateBounds();
        }
    }

    Rectangle UIElement::GetParentBounds() const
    {
        if (m_Parent == nullptr)
            return Instance->ScreenRect;

        return m_Parent->GetBounds();
    }
}
