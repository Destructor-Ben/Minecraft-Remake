#include "Logger.h"
#include "Game.h"

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
        Throw(GetMessage(message, "ERROR"));
    }

    void Logger::Log(const string& message)
    {
        // TODO: output to file
        std::cout << message << "\n";
    }

    void Logger::Throw(const string& message)
    {
        //throw std::exception(message.c_str());  commented as this causes problems on my system
    }

    string Logger::GetMessage(const string& message, const string& logLevel)
    {
        auto time = std::chrono::system_clock::now();
        string timeString = format("{:%H:%M:}{:%S}", time, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()));

        auto threadID = std::this_thread::get_id();
        string threadName = threadID == MainThreadID ? "Main Thread" : threadID == TickThread->get_id() ? "Tick Thread" : "Unknown Thread";

        return format("[{}] [{}/{}] {}", timeString, threadName, logLevel, message);
    }
}
