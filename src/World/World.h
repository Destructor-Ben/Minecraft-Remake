#pragma once

#include "../Common.h"

#include "../Graphics/Camera.h"

namespace Minecraft
{
    class World
    {
    public:
        Camera Camera;

        void OnEnter();
        void OnExit();

        void Tick();
        void Update();
        void Render();

    private:
        void UpdateCamera();
    };
}
