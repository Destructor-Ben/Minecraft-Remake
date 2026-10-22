#pragma once

#include "Config.h"
#include "World/State/SkyState.h"
#include "World/State/TimeState.h"

namespace Minecraft::SkySystem
{
    void Update(SkyState &sky, TimeState &time);
    void UpdateFromConfig(SkyState &sky, Config &config);
}
