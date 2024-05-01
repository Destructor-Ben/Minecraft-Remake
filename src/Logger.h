#pragma once

#include "Common.h"

namespace Minecraft
{
    class Logger
    {
    public:
        void Debug(const string& message);
        void Info(const string& message);
        void Warn(const string& message);
        void Error(const string& message);

    private:
        void Log(const string& message);
        void Throw(const string& message);
        string GetMessage(const string& message, const string& logLevel);
    };
}
