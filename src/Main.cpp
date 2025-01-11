#include <csignal>

#include "Game.h"
#include "LogManager.h"

using namespace Minecraft;

// Super important to null out Logger, since we want it to deallocate and write to the log file
#define HANDLE_EXCEPTION(function) \
        Instance->Logger->function;\
        Instance->Logger = nullptr;\
        return -1;

// Let OSes shut this bitch down
void SignalHandler(int signal)
{
    Instance->Logger->Info(format("Signal received: {}", signal));
    Instance->Close();
}

int main()
{
    try
    {
        std::signal(SIGINT, SignalHandler);
        std::signal(SIGTERM, SignalHandler);

        Instance = make_shared<Game>();
        Instance->Initialize();
        Instance->Run();
        Instance->Shutdown();
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
