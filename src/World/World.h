#pragma once

#include "../Common.h"

#include <map>

#include "Chunk.h"
#include "../Graphics/Camera.h"

namespace Minecraft
{
    class World
    {
    public:
        Camera Camera;

        void OnEnter();
        void OnExit();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        // TODO: entity getters
        // TODO: block getters

    private:
        void UpdateCamera();

        std::map<uint32, Chunk> m_Chunks;
    };
}
