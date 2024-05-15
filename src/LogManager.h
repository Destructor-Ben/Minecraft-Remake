#pragma once

#include "Common.h"

namespace Minecraft
{
    // TODO: use cerr/clog instead of cout
    // TODO: make debug errors only happen in debug builds
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
        void Log(const string& message);
        static string GetMessage(const string& message, const string& logLevel);

        std::ofstream m_LogFile = {};
    };
}
