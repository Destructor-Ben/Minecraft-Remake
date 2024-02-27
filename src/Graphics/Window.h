#pragma once

#include "../Common.h"

namespace Minecraft
{
    namespace Window
    {
        const int InitialWidth = 1280;
        const int InitialHeight = 720;
        const bool StartFullScreen = true;
        const str Title = "Minecraft";

        extern GLFWwindow* Handle;
        extern int ScreenWidth;
        extern int ScreenHeight;

        inline void Close()
        {
            glfwSetWindowShouldClose(Window::Handle, true);
        }
    }
}