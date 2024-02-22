#pragma once

#include "Common.h"

namespace Minecraft
{
    const int InitialWidth = 1280;
    const int InitialHeight = 720;
    const bool StartFullscreen = true;

    // TODO: capitalize this
    extern GLFWwindow* window;
    // TODO: add window size variables

    void Initialize();
    void Shutdown();

    void Tick(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void OnResize(int width, int height);
}
