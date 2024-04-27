#pragma once

#include "src/Common.h"

namespace Minecraft
{
    class Logger
    {
    public:
        void Debug(const string& message);
        void Info(const string& message);
        void Warn(const string& message);
        void Error(const string& message);

        void GLFWError(int32 code, cstring description);
        void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam);

    private:
        void Log(const string& message);
        void Throw(const string& message);
    };
}
