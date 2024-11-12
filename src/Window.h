#pragma once

namespace Minecraft
{
    namespace Window
    {
        // User configurable settings
        inline float TargetFrameRate = 60;
        inline float TargetTickRate = 10;
        inline bool VSyncEnabled = false; // Change with the setter

        // Updated as the application runs
        inline GLFWwindow* Handle = nullptr;
        inline int Width = 0;
        inline int Height = 0;

        inline void Close() { glfwSetWindowShouldClose(Window::Handle, true); }

        inline void SetVSyncEnabled(bool value)
        {
            glfwSwapInterval(value ? 1 : 0);
            VSyncEnabled = value;
        }
    }
}
