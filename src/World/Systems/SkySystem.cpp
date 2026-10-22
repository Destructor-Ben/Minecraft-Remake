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
        sky.MoonLight = vec3(0.3f) * sky.Darkness;
    }

    void UpdateConfig(SkyState &sky, Config &config)
    {
        sky.Config.BrightnessFadeDuration = config.TryGetValue<float>("brightness-fade-duration").value_or(0.0);

        sky.Config.SunsetStrengthMultiplier = config.TryGetValue<float>("sunset/strength-multiplier").value_or(1.0);
        sky.Config.SunsetConstantDuration = config.TryGetValue<float>("sunset/constant-duration").value_or(0.0);
        sky.Config.SunsetFadeDuration = config.TryGetValue<float>("sunset/fade-duration").value_or(0.0);
        sky.Config.SunsetAngle = config.TryGetValue<float>("sunset/angle").value_or(0.0);
        sky.Config.SunsetCoverage = config.TryGetValue<float>("sunset/coverage").value_or(0.0);
    }

    static void UpdateSkyDarkness(SkyState &sky, float timePercent)
    {
        float skyDarkness = 1;

        if (timePercent <= TimeState::Dusk)
            skyDarkness = 0;

        // Common linear equation values
        float gradient = 1 / sky.Config.BrightnessFadeDuration;
        float interceptNightStart = -gradient * 0.5;
        float interceptNightEnd = gradient;

        // After dusk start fading in
        if (TimeState::Dusk < timePercent && timePercent < TimeState::Dusk + sky.Config.BrightnessFadeDuration)
            skyDarkness = gradient * timePercent + interceptNightStart;

        // Before dawn start fading out
        if (TimeState::EndOfDay - sky.Config.BrightnessFadeDuration < timePercent && timePercent < TimeState::EndOfDay)
            skyDarkness = -gradient * timePercent + interceptNightEnd;

        sky.Darkness = skyDarkness;
        sky.Brightness = 1.0f - skyDarkness;
    }

    static void UpdateSunset(SkyState &sky, float timePercent)
    {
        // Sunsets will fade in linearly, stay for a bit, then fade out
        float fadeTime = sky.Config.SunsetFadeDuration;
        float sunsetTime = sky.Config.SunsetConstantDuration;
        float halfSunsetTime = sunsetTime / 2.0f;

        float sunsetStrength = 0;

        if (timePercent <= halfSunsetTime || timePercent >= 1 - halfSunsetTime || (timePercent >= 0.5f - halfSunsetTime && timePercent <= 0.5f + halfSunsetTime))
            sunsetStrength = 1;

        // Common linear equation values
        float gradient = 1 / fadeTime;
        float interceptSunsetStart = 1 - gradient * (0.5 - halfSunsetTime);
        float interceptSunsetEnd = 1 + gradient * (0.5 + halfSunsetTime);
        float interceptSunriseStart = 1 - gradient * (1 - halfSunsetTime);
        float interceptSunriseEnd = 1 + gradient * (halfSunsetTime);

        // Starting sunset
        if (timePercent > TimeState::Dusk - halfSunsetTime - fadeTime && timePercent < TimeState::Dusk - halfSunsetTime)
            sunsetStrength = gradient * timePercent + interceptSunsetStart;

        // Ending sunset
        if (timePercent < TimeState::Dusk + halfSunsetTime + fadeTime && timePercent > TimeState::Dusk + halfSunsetTime)
            sunsetStrength = -gradient * timePercent + interceptSunsetEnd;

        // Starting sunrise
        if (timePercent > TimeState::EndOfDay - halfSunsetTime - fadeTime && timePercent < TimeState::EndOfDay - halfSunsetTime)
            sunsetStrength = gradient * timePercent + interceptSunriseStart;

        // Ending sunrise
        if (timePercent < TimeState::Dawn + halfSunsetTime + fadeTime && timePercent > TimeState::Dawn + halfSunsetTime)
            sunsetStrength = -gradient * timePercent + interceptSunriseEnd;

        // Make the sunset more visible
        sky.SunsetStrength = sunsetStrength * sky.Config.SunsetStrengthMultiplier;

        float realAngle = glm::radians(90.0f - sky.Config.SunsetAngle); // We need 90 - Angle because of the working out on paper
        sky.SunsetDirection = vec3(-cos(realAngle), -sin(realAngle), 0);

        // Adjust for east/west with rise/set
        sky.SunsetDirection.x *= timePercent <= TimeState::Midnight && timePercent >= TimeState::Noon ? 1 : -1;
    }
}
