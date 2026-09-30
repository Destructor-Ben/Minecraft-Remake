#include "SkySystem.h"
#include "World/State/TimeState.h"

namespace Minecraft::SkySystem
{
    static void UpdateSkyDarkness(SkyState &sky, float timePercent);
    static void UpdateSunset(SkyState &sky, float timePercent);

    // TODO: colour the sun and moon lighting, similar to how the glow is coloured, and also change it based on the sunset values
    void Update(SkyState &sky, TimeState &time)
    {
        UpdateSkyDarkness(sky, time.TimePercent);
        UpdateSunset(sky, time.TimePercent);

        sky.CelestialObjectAngle = time.TimePercent * 2 * numbers::pi;
        sky.CelestialObjectRotation = glm::eulerAngleZ(sky.CelestialObjectAngle);
        sky.DirToSun = sky.CelestialObjectRotation * vec4(1, 0, 0, 1);
        sky.DirToMoon = -sky.DirToSun;

        // TODO: no hardcode, load from config
        sky.AmbientLight = vec3(glm::lerp(0.1f, 0.5f, sky.Brightness));
        sky.SunLight = vec3(0.5f) * sky.Brightness;
        sky.MoonLight = vec3(0.3f) * sky.Brightness;
    }

    static void UpdateSkyDarkness(SkyState &sky, float timePercent)
    {
        // TODO: no hardcode, load from config
        constexpr float FadeTime = 0.05f;

        float skyDarkness = 1;

        if (timePercent <= TimeState::Dusk)
            skyDarkness = 0;

        // Common linear equation values
        float gradient = 1 / FadeTime;
        float interceptNightStart = -gradient * 0.5;
        float interceptNightEnd = gradient;

        // After dusk start fading in
        if (TimeState::Dusk < timePercent && timePercent < TimeState::Dusk + FadeTime)
            skyDarkness = gradient * timePercent + interceptNightStart;

        // Before dawn start fading out
        if (TimeState::EndOfDay - FadeTime < timePercent && timePercent < TimeState::EndOfDay)
            skyDarkness = -gradient * timePercent + interceptNightEnd;

        sky.Darkness = skyDarkness;
        sky.Brightness = 1.0f - skyDarkness;
    }

    static void UpdateSunset(SkyState &sky, float timePercent)
    {
        // Sunsets will fade in linearly, stay for a bit, then fade out
        // TODO: no hardcode, load from config
        constexpr float FadeTime = 0.075;
        constexpr float SunsetTime = 0.025f;
        constexpr float HalfSunsetTime = SunsetTime / 2.0f;

        float sunsetStrength = 0;

        if (timePercent <= HalfSunsetTime || timePercent >= 1 - HalfSunsetTime || (timePercent >= 0.5f - HalfSunsetTime && timePercent <= 0.5f + HalfSunsetTime))
            sunsetStrength = 1;

        // Common linear equation values
        float gradient = 1 / FadeTime;
        float interceptSunsetStart = 1 - gradient * (0.5 - HalfSunsetTime);
        float interceptSunsetEnd = 1 + gradient * (0.5 + HalfSunsetTime);
        float interceptSunriseStart = 1 - gradient * (1 - HalfSunsetTime);
        float interceptSunriseEnd = 1 + gradient * (HalfSunsetTime);

        // Starting sunset
        if (timePercent > TimeState::Dusk - HalfSunsetTime - FadeTime && timePercent < TimeState::Dusk - HalfSunsetTime)
            sunsetStrength = gradient * timePercent + interceptSunsetStart;

        // Ending sunset
        if (timePercent < TimeState::Dusk + HalfSunsetTime + FadeTime && timePercent > TimeState::Dusk + HalfSunsetTime)
            sunsetStrength = -gradient * timePercent + interceptSunsetEnd;

        // Starting sunrise
        if (timePercent > TimeState::EndOfDay - HalfSunsetTime - FadeTime && timePercent < TimeState::EndOfDay - HalfSunsetTime)
            sunsetStrength = gradient * timePercent + interceptSunriseStart;

        // Ending sunrise
        if (timePercent < TimeState::Dawn + HalfSunsetTime + FadeTime && timePercent > TimeState::Dawn + HalfSunsetTime)
            sunsetStrength = -gradient * timePercent + interceptSunriseEnd;

        // Make the sunset more visible
        sunsetStrength *= 2;
        
        sky.SunsetStrength = sunsetStrength;
        // TODO: no hardcode angle or coverage, load from config (and store in other values, which are then used here in calculations)
        sky.SunsetCoverage = 0.25;
        constexpr float Angle = 15.0f;
        float angle = glm::radians(90.0f - Angle); // We need 90 - Angle because of the working out on paper
        sky.SunsetDirection = vec3(-cos(angle), -sin(angle), 0);

        // Adjust for east/west with rise/set
        sky.SunsetDirection.x *= timePercent <= TimeState::Midnight && timePercent >= TimeState::Noon ? 1 : -1;
    }
}
