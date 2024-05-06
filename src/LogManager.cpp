#include "LogManager.h"

#include "Game.h"

namespace Minecraft
{
    LogManager::LogManager()
    {
        m_LogFile.open("Minecraft_Remake.log", std::ofstream::out | std::ofstream::trunc);
        m_LogFile << "Minecraft_Remake log file, version " << Version::String << "\n\n";
    }

    LogManager::~LogManager()
    {
        m_LogFile.close();
    }

    void LogManager::Debug(const string& message)
    {
        Log(GetMessage(message, "DEBUG"));
    }

    void LogManager::Info(const string& message)
    {
        Log(GetMessage(message, "INFO"));
    }

    void LogManager::Warn(const string& message)
    {
        Log(GetMessage(message, "WARN"));
    }

    void LogManager::Error(const string& message)
    {
        Log(GetMessage(message, "ERROR"));
    }

    void LogManager::Throw(const string& message)
    {
        throw std::runtime_error(message);
    }

    void LogManager::CatchUnknown()
    {
        Catch("Unknown exception occurred");
    }

    void LogManager::Catch(const string& message)
    {
        Log(GetMessage(message, "FATAL"));
    }

    void LogManager::Catch(const std::exception& exception)
    {
        Catch(exception.what());
    }

    void LogManager::Log(const string& message)
    {
        std::cout << message;
        m_LogFile << message;
    }

    string LogManager::GetMessage(const string& message, const string& logLevel)
    {
        auto time = std::chrono::system_clock::now();
        string timeString = format("{:%H:%M:}{:%S}", time, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()));

        auto threadID = std::this_thread::get_id();
        string threadName = threadID == MainThreadID ? "Main Thread" : threadID == TickThread->get_id() ? "Tick Thread" : "Unknown Thread";

        return format("[{}] [{}/{}] {}\n", timeString, threadName, logLevel, message);
    }
}
