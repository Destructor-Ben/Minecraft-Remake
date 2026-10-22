#pragma once

#include "Config.h"
#include "World/State/SkyState.h"
#include "World/State/TimeState.h"

namespace Minecraft::SkySystem
{
    void Update(SkyState &sky, TimeState &time);
    void UpdateConfig(SkyState &sky, Config &config);
}
