#version 330 core

in vec3 FragCoord;

uniform float uTime;
uniform sampler2D uDayGradient;
uniform sampler2D uNightGradient;
uniform sampler2D uSunsetGradient;

out vec4 FragColor;

void main()
{
    vec3 coord = normalize(FragCoord);

    // Lerp the color based on y value of the coord
    // Idk why but the y at the top is negative
    vec2 gradientCoord = vec2(1.0 - clamp(0.0, 1.0, coord.y), 0.5);

    // TODO: clean up this code
    vec3 dayCol = texture(uDayGradient, gradientCoord).rgb;
    vec3 nightCol = texture(uNightGradient, gradientCoord).rgb;
    // TODO: maybe rotate the gradient a little
    vec3 sunsetCol = texture(uSunsetGradient, gradientCoord).rgb;

    vec3 col;
    if (uTime < 0.25) // Dawn to noon
    {
        float time = uTime * 4.0;
        col = mix(sunsetCol, dayCol, time);
    }
    else if (uTime < 0.5) // Noon to dusk
    {
        float time = (uTime - 0.25) * 4.0;
        col = mix(dayCol, sunsetCol, time);
    }
    else if (uTime < 0.75) // Dusk to midnight
    {
        float time = (uTime - 0.5) * 4.0;
        col = mix(sunsetCol, nightCol, time);
    }
    else // Midnight to dawn
    {
        float time = (uTime - 0.75) * 4.0;
        col = mix(nightCol, sunsetCol, time);
    }

    FragColor = vec4(col, 1.0);
}
