#include "LogManager.h"

#include <cpptrace/cpptrace.hpp>

#include "Version.h"

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
        Log(GetMessage(message, "ERROR"), true);
    }

    void LogManager::Throw(const string& message)
    {
        throw std::runtime_error(format("{}\n{}", message, cpptrace::generate_trace().to_string()));
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
