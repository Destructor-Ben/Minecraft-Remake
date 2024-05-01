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

    void Logger::Log(const string& message)
    {
        // TODO: output to file
        std::cout << message << "\n";
    }

    void Logger::Throw(const string& message)
    {
        throw std::exception(message.c_str());
    }

    string Logger::GetMessage(const string& message, const string& logLevel)
    {
        // TODO: thread name
        auto time = std::chrono::system_clock::now();
        string timeString = format("{:%H:%M:}{:%S}", time, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()));
        string threadName = "Thread Name";
        return format("[{}] [{}/{}] {}", timeString, threadName, logLevel, message);
    }
}
