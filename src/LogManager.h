#pragma once

namespace Minecraft
{
    class LogManager
    {
    public:
        LogManager();
        ~LogManager();

        void Debug(const string& message);
        void Info(const string& message);
        void Warn(const string& message);
        void Error(const string& message);
        void Throw(const string& message);
        void Assert(bool condition);

        void CatchUnknown();
        void Catch(const string& message);
        void Catch(const std::exception& exception);

        static void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam);
        static void GLFWError(int code, cstring description);

    private:
        void Log(const string& message, bool error = false);
        static string GetMessage(const string& message, const string& logLevel);

        std::ofstream m_LogFile = { };
    };
}
