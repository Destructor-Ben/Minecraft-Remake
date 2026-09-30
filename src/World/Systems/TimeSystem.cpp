#include "TimeSystem.h"

#include "Input/Input.h"

namespace Minecraft
{
    void TimeSystem::Tick(float deltaTime, TimeState &time)
    {
        // Debug, will probably be changed in the future: [[
        if (Input::WasKeyReleased(Key::P))
            time.TimeSeconds = TimeState::Noon;

        if (Input::WasKeyReleased(Key::L))
            time.TimeSeconds = TimeState::Midnight;
        // ]]

        time.TimeSeconds += deltaTime;

        // New days start after dawn, not midnight
        if (time.TimeSeconds >= TimeState::DayLength)
        {
            time.TimeSeconds -= TimeState::DayLength;
            time.DayCount++;
        }

        time.TimePercent = time.TimeSeconds / TimeState::DayLength;
        time.IsDay = time.TimeSeconds > TimeState::Dawn && time.TimeSeconds < TimeState::Dusk;
    }
}
