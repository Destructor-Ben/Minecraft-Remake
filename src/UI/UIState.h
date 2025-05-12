#pragma once

namespace Minecraft
{
    class UIElement;

    // Root level ui object
    class UIState
    {
    public:
        bool Active = true;

        void Register();

        void Update();
        void Render();

        void AddElement(shared_ptr<UIElement> element);
        void RemoveElement(shared_ptr<UIElement> element);

    private:
        vector<shared_ptr<UIElement>> m_Elements = { };
    };
}
