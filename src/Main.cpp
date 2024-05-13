#include "Game.h"

using namespace Minecraft;

// Forward declare functions from Game.cpp so they arne't in the header and pollute the namespace
namespace Minecraft
{
    void Initialize();
    void Run();
    void Shutdown();
}

// Super important to null out Logger, since we want it to deallocate and write to the log file
#define HANDLE_EXCEPTION(function)\
        Logger->function;\
        Logger = nullptr;\
        return -1;                \

int main()
{
    try
    {
        Initialize();
        Run();
        Shutdown();
    }
    catch (const std::exception& exception)
    {
        HANDLE_EXCEPTION(Catch(exception))
    }
    catch (const string& message)
    {
        HANDLE_EXCEPTION(Catch(message))
    }
    catch (const cstring& message)
    {
        HANDLE_EXCEPTION(Catch(message))
    }
    catch (...)
    {
        HANDLE_EXCEPTION(CatchUnknown())
    }

    return 0;
}
