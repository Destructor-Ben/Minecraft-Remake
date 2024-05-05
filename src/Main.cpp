#include "Game.h"

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
        Logger->Catch(exception);
        return -1;
    }
    catch (const string& message)
    {
        Logger->Catch(message);
        return -1;
    }
    catch (...)
    {
        Logger->CatchUnknown();
        return -1;
    }

    return 0;
}
