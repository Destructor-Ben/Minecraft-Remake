#pragma once

#include "Common.h"
#include "Input/Input.h"

namespace Minecraft
{
    const int InitialWidth = 1280;
    const int InitialHeight = 720;
    const bool StartFullScreen = true;

    extern GLFWwindow* Window;
    extern int ScreenWidth;
    extern int ScreenHeight;

    extern InputManager Input;

    void Initialize();
    void Shutdown();

    void Tick(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void OnResize(int width, int height);
}
