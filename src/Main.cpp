#include "Game.h"
#include "LogManager.h"

using namespace Minecraft;

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
        // Super important to null out Logger, since we want it to deallocate and write to the log file
        Logger->Catch(exception);
        Logger = nullptr;
        return -1;
    }
    catch (const string& message)
    {
        Logger->Catch(message);
        Logger = nullptr;
        return -1;
    }
    catch (const cstring& message)
    {
        Logger->Catch(message);
        Logger = nullptr;
        return -1;
    }
    catch (...)
    {
        Logger->CatchUnknown();
        Logger = nullptr;
        return -1;
    }

    return 0;
}
