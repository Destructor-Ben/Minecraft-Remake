#pragma once

#include "Rectangle.h"
#include "UI/UIDimension.h"

namespace Minecraft
{
    // Basic element of UI
    class UIElement
    {
    public:
        bool Active = true;

        // Dimensions
        UIDimension x;
        UIDimension y;

        UIDimension Width;
        UIDimension Height;

        UIDimension OriginX;
        UIDimension OriginY;

        UIElement() { }

        void AddElement(shared_ptr <UIElement> element);
        void RemoveElement(shared_ptr <UIElement> element);

        void Update();
        void Render();
        void ScreenResized();

        virtual void RecalculateBounds();

        // None of these need any base function to be called
        virtual void OnAdd() { }
        virtual void OnRemove() { }
        virtual void OnResize() { }

        virtual void OnUpdate() { }
        virtual void OnRender() { }

        Rectangle GetParentBounds() const;
        Rectangle GetBounds() const { return m_Bounds; }
        vec2i GetPosition() const { return m_Bounds.GetPosition(); }
        vec2i GetSize() const { return m_Bounds.GetSize(); }
        vec2i GetOrigin() const { return m_Origin; }

        UIElement* GetParent() const { return m_Parent; }
        const vector <shared_ptr<UIElement>>& GetChildren() const { return m_Children; }

    private:
        UIElement* m_Parent = nullptr;
        vector <shared_ptr<UIElement>> m_Children = { };

        Rectangle m_Bounds = { };
        vec2i m_Origin = { };
    };
}
