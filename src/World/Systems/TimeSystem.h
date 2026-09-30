#pragma once

#include "World/State/TimeState.h"

namespace Minecraft::TimeSystem
{
    void Tick(float deltaTime, TimeState &time);
}
