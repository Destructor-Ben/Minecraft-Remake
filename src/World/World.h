#pragma once

#include "../Common.h"

namespace Minecraft
{
    class World
    {
    public:
        glm::vec3 CameraPosition = glm::vec3(0.0f);
        glm::vec2 CameraAngles = glm::vec2(0.0f);

        void OnEnter();
        void OnExit();

        void Tick();
        void Update();
        void Render();

    private:
        void UpdateCamera();
    };
}
