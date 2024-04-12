#pragma once

#include "../Common.h"

namespace Minecraft
{
    namespace Window
    {
        const int InitialWidth = 1280;
        const int InitialHeight = 720;
        const string Title = "Minecraft";

        extern GLFWwindow* Handle;
        extern int Width;
        extern int Height;

        inline void Close()
        {
            glfwSetWindowShouldClose(Window::Handle, true);
        }
    }
}
