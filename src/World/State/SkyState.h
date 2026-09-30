#pragma once

namespace Minecraft
{
    struct SkyState
    {
        float Brightness = 0;
        float Darkness = 0;

        float SunsetStrength = 0;
        float SunsetCoverage = 0;
        vec3 SunsetDirection = { };

        float CelestialObjectAngle = { };
        mat4 CelestialObjectRotation = { };
        vec3 DirToSun = { };
        vec3 DirToMoon = { };

        vec3 SunLight = { };
        vec3 MoonLight = { };
        vec3 AmbientLight = { };
    };
}
