#include "Logger.h"

namespace Minecraft
{

    void Logger::Debug(const string& message)
    {
        Log(GetMessage(message, "DEBUG"));
    }

    void Logger::Info(const string& message)
    {
        Log(GetMessage(message, "INFO"));
    }

    void Logger::Warn(const string& message)
    {
        Log(GetMessage(message, "WARN"));
    }

    void Logger::Error(const string& message)
    {
        string error = GetMessage(message, "ERROR");
        Log(error);
        Throw(error);
    }

    void Logger::GLFWError(int32 code, cstring description)
    {
        Warn(format("GLFW Error ({}): {}", code, description));
    }

    void Logger::GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam)
    {
        // TODO: improve GLErrors
        Warn(format("GL Error:\n  Source: {}\n  Type: {}\n  ID: {}\n  Severity: {}\n  Message: {}", source, type, id, severity, message));
    }

    void Logger::Log(const string& message)
    {
        std::cout << message << "\n";
    }

    void Logger::Throw(const string& message)
    {
        throw std::exception(message.c_str());
    }

    string Logger::GetMessage(const string& message, const string& logLevel)
    {
        // TODO: time and thread name
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return logLevel + " - " + message;//format("[{}] [{}/{}] {}", std::ctime(&time), std::this_thread::get_id(), logLevel, message);
    }
}
