#pragma once

#include "Color.h"

namespace Minecraft
{
    struct SkyState
    {
        float Brightness = 0;
        float Darkness = 0;

        float SunsetStrength = 0;
        vec3 SunsetDirection = { };

        float CelestialObjectAngle = { };
        mat4 CelestialObjectRotation = { };
        vec3 DirToSun = { };
        vec3 DirToMoon = { };

        vec3 SunLight = { };
        vec3 MoonLight = { };
        vec3 AmbientLight = { };

        struct SkyConfig {
            float BrightnessFadeDuration = 0;

            struct {
                Color MainColor = { };
                float StrengthMultiplier = 0;
                float ConstantDuration = 0;
                float FadeDuration = 0;
                float Coverage = 0;
                float Angle = 0;
            } Sunset;

            struct {
                int Count = 0;
                ulong Seed = 0;
                float TemperatureStrength = 0;
                float MaxBrightness = 0;
                float TwinkleStrength = 0;
                float MinTwinkleSpeed = 0;
                float MaxTwinkleSpeed = 0;
                float Scale = 0;
                float ScalePercentVariance = 0;
            } Stars;
        } Config;
    };
}
