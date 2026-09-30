#pragma once

#include "World/State/PlayerState.h"
#include "World/World.h"

namespace Minecraft::PlayerSystem
{
    void Tick(PlayerState &player, World &world, float deltaTime);
    void Update(PlayerState &player, int screenWidth, int screenHeight);
    void OnEnterWorld(PlayerState &player);
}
