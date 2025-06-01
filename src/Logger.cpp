#include "Logger.h"

#include <cpptrace/cpptrace.hpp>

#include "Version.h"

namespace Minecraft::Logger
{
    std::ofstream LogFile = { };

    static void Log(const string& message, bool error = false);
    static string GetMessage(const string& message, const string& logLevel);

    void Init()
    {
        LogFile.open("Minecraft_Remake.log", std::ofstream::out | std::ofstream::trunc);
        LogFile << "Minecraft_Remake log file, version " << Version::String << "\n\n" << std::flush;
    }

    void Shutdown()
    {
        LogFile.close();
    }

    void Debug(const string& message)
    {
        Log(GetMessage(message, "DEBUG"));
    }

    void Info(const string& message)
    {
        Log(GetMessage(message, "INFO"));
    }

    void Warn(const string& message)
    {
        Log(GetMessage(message, "WARN"));
    }

    void Error(const string& message)
    {
        Log(GetMessage(message, "ERROR"), true);
    }

    void Throw(const string& message)
    {
        throw std::runtime_error(format("{}\n{}", message, cpptrace::generate_trace().to_string()));
    }

    void Assert(bool condition)
    {
        if (!condition)
            Throw("Assertion failed");
    }

    void CatchUnknown()
    {
        Catch("Unknown exception occurred");
    }

    void Catch(const string& message)
    {
        Log(GetMessage(message, "FATAL"), true);
    }

    void Catch(const std::exception& exception)
    {
        Catch(exception.what());
    }

    void GLFWError(int code, cstring description)
    {
        Error(format("GLFW Error (Code {}): {}", code, description));
    }

    void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam)
    {
        string sourceString;
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:
                sourceString = "API";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                sourceString = "Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                sourceString = "Other";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                sourceString = "Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                sourceString = "Third Party";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                sourceString = "Window System";
                break;
            default:
                Throw("Unknown GL Error source: " + to_string(source));
                break;
        }

        string typeString;
        switch (type)
        {
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                typeString = "Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_ERROR:
                typeString = "Error";
                break;
            case GL_DEBUG_TYPE_MARKER:
                typeString = "Marker";
                break;
            case GL_DEBUG_TYPE_OTHER:
                typeString = "Other";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                typeString = "Performance";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                typeString = "Pop Group";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                typeString = "Portability";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                typeString = "Push Group";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                typeString = "Undefined Behaviour";
                break;
            default:
                Throw("Unknown GL Error type: " + to_string(source));
                break;
        }

        string severityString;
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                severityString = "High";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                severityString = "Medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                severityString = "Low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                severityString = "Notification";
                break;
            default:
                Throw("Unknown GL Error severity: " + to_string(source));
                break;
        }

        string logMessage = format("GL Error: {}\n  Source: {}\n  Type: {}\n  Severity: {}\n  ID: {}", message, sourceString, typeString, severityString, id);

        if (severity == GL_DEBUG_SEVERITY_HIGH)
            Error(logMessage);
        else
            Warn(logMessage);
    }

    void Log(const string& message, bool error)
    {
        (error ? std::cerr : std::cout) << message;
        LogFile << message;

        // Flush buffer if it's an error
        if (error)
            LogFile << std::flush;
    }

    string GetMessage(const string& message, const string& logLevel)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowCTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&nowCTime);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        string timeString = format(
            "{:02}:{:02}:{:02}.{:03}",
            localTime.tm_hour,
            localTime.tm_min,
            localTime.tm_sec,
            milliseconds.count()
        );

        return format("[{}] [{}] {}\n", timeString, logLevel, message);
    }
}
