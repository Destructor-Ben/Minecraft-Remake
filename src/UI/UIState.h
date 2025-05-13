#pragma once

namespace Minecraft
{
    class UIElement;

    // Root level ui object
    class UIState
    {
    public:
        bool Active = true;
        bool IsInGameUI = true;

        virtual void Init() { };

        // Make sure the UI isn't active when it isn't meant to be
        // Always called even if not active
        virtual void CheckActive();
        virtual void Update();
        virtual void Render();

        void AddElement(shared_ptr<UIElement> element);
        void RemoveElement(shared_ptr<UIElement> element);

    private:
        vector<shared_ptr<UIElement>> m_Elements = { };
    };
}
