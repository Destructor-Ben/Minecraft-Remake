#pragma once

namespace Minecraft::Logger
{
    void Init();
    void Shutdown();

    void Debug(const string& message);
    void Info(const string& message);
    void Warn(const string& message);
    void Error(const string& message);
    void Throw(const string& message);
    void Assert(bool condition);

    void CatchUnknown();
    void Catch(const string& message);
    void Catch(const std::exception& exception);

    void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam);
    void GLFWError(int code, cstring description);
}
