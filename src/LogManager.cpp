#include "LogManager.h"

#include <stacktrace>

#include "Game.h"

namespace Minecraft
{
    LogManager::LogManager()
    {
        m_LogFile.open("Minecraft_Remake.log", std::ofstream::out | std::ofstream::trunc);
        m_LogFile << "Minecraft_Remake log file, version " << Version::String << "\n\n" << std::flush;
    }

    LogManager::~LogManager()
    {
        m_LogFile.close();
    }

    void LogManager::Debug(const string& message)
    {
        #if DEBUG
        Log(GetMessage(message, "DEBUG"));
        #endif
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
        Log(GetMessage(message, "ERROR"), true);
    }

    void LogManager::Throw(const string& message)
    {
        throw std::runtime_error(format("{}\nStacktrace:\n{}", message, to_string(std::stacktrace::current())));
    }

    void LogManager::Assert(bool condition)
    {
        if (!condition)
            Throw("Assertion failed");
    }

    void LogManager::CatchUnknown()
    {
        Catch("Unknown exception occurred");
    }

    void LogManager::Catch(const string& message)
    {
        Log(GetMessage(message, "FATAL"), true);
    }

    void LogManager::Catch(const std::exception& exception)
    {
        Catch(exception.what());
    }

    void LogManager::Log(const string& message, bool error)
    {
        (error ? std::cerr : std::cout) << message;
        m_LogFile << message;

        // Flush buffer if it's an error
        if (error)
            m_LogFile << std::flush;
    }

    string LogManager::GetMessage(const string& message, const string& logLevel)
    {
        auto time = chrono::system_clock::now();
        string timeString = format("{:%H:%M:}{:%S}", time, chrono::duration_cast<chrono::milliseconds>(time.time_since_epoch()));

        auto threadID = std::this_thread::get_id();
        string threadName = threadID == MainThreadID ? "Main Thread" : threadID == TickThread->get_id() ? "Tick Thread" : "Unknown Thread";

        return format("[{}] [{}/{}] {}\n", timeString, threadName, logLevel, message);
    }
}
