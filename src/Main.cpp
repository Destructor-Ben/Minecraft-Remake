#include "Game.h"
#include "LogManager.h"

using namespace Minecraft;

// Super important to null out Logger, since we want it to deallocate and write to the log file
#define HANDLE_EXCEPTION(function) \
        Instance->Logger->function;\
        Instance->Logger = nullptr;\
        return -1;

int main()
{
    try
    {
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
