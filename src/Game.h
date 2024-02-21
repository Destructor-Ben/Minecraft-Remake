#pragma once

#include "Common.h"

namespace Minecraft
{
    extern GLFWwindow* window;

    void Initialize();
    void Shutdown();

    void Tick(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void OnResize(int width, int height);
}
