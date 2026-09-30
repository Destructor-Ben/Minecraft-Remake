#include "TimeSystem.h"

#include "Input/Input.h"
#include "World/State/TimeState.h"

namespace Minecraft::TimeSystem
{
    void Tick(TimeState &time, float deltaTime)
    {
        // Debug, will probably be changed in the future: [[
        if (Input::WasKeyReleased(Key::P))
            time.TimeSeconds = TimeState::Noon * TimeState::DayLength;

        if (Input::WasKeyReleased(Key::L))
            time.TimeSeconds = TimeState::Midnight * TimeState::DayLength;

        if (Input::WasKeyReleased(Key::O))
            time.TimeSeconds = TimeState::Dawn * TimeState::DayLength;

        if (Input::WasKeyReleased(Key::K))
            time.TimeSeconds = TimeState::Dusk * TimeState::DayLength;
        // ]]

        time.TimeSeconds += deltaTime;

        // New days start after dawn, not midnight
        if (time.TimeSeconds >= TimeState::DayLength)
        {
            time.TimeSeconds -= TimeState::DayLength;
            time.DayCount++;
        }

        time.TimePercent = time.TimeSeconds / TimeState::DayLength;
        time.IsDay = time.TimePercent > TimeState::Dawn && time.TimePercent < TimeState::Dusk;
    }
}
