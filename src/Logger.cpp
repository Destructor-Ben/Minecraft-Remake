#include "Logger.h"
#include "Game.h"

namespace Minecraft
{
    Logger_t::Logger_t()
    {
        m_LogFile.open("Minecraft_Remake.log", std::ofstream::out | std::ofstream::trunc);
    }

    Logger_t::~Logger_t()
    {
        m_LogFile.close();
    }

    void Logger_t::Debug(const string& message)
    {
        Log(GetMessage(message, "DEBUG"));
    }

    void Logger_t::Info(const string& message)
    {
        Log(GetMessage(message, "INFO"));
    }

    void Logger_t::Warn(const string& message)
    {
        Log(GetMessage(message, "WARN"));
    }

    void Logger_t::Error(const string& message)
    {
        Log(GetMessage(message, "ERROR"));
    }

    void Logger_t::Throw(const string& message)
    {
        throw std::exception(message.c_str());
    }

    void Logger_t::CatchUnknown()
    {
        Catch("Unknown exception occurred");
    }

    void Logger_t::Catch(const string& message)
    {
        Log(GetMessage(message, "FATAL"));
    }

    void Logger_t::Catch(const std::exception& exception)
    {
        Catch(exception.what());
    }

    void Logger_t::Log(const string& message)
    {
        std::cout << message;
        m_LogFile << message;
    }

    string Logger_t::GetMessage(const string& message, const string& logLevel)
    {
        auto time = std::chrono::system_clock::now();
        string timeString = format("{:%H:%M:}{:%S}", time, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()));

        auto threadID = std::this_thread::get_id();
        string threadName = threadID == MainThreadID ? "Main Thread" : threadID == TickThread->get_id() ? "Tick Thread" : "Unknown Thread";

        return format("[{}] [{}/{}] {}\n", timeString, threadName, logLevel, message);
    }
}
