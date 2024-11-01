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

    private:
        void Log(const string& message, bool error = false);
        static string GetMessage(const string& message, const string& logLevel);

        std::ofstream m_LogFile = { };
    };
}
