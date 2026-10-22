#pragma once

namespace Minecraft
{
    // TODO: separate state from config?
    struct SkyState
    {
        float Brightness = 0;
        float Darkness = 0;
        float BrightnessFadeDuration = 0;

        float SunsetStrength = 0;
        float SunsetStrengthMultiplier = 0;
        float SunsetConstantDuration = 0;
        float SunsetFadeDuration = 0;
        float SunsetCoverage = 0;
        float SunsetAngle = 0;
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
