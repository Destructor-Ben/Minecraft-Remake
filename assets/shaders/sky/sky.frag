#version 330 core

in vec3 FragCoord;

uniform float uSkyDarkness;
uniform float uSunsetStrength;
uniform float uSunsetCoverage;
uniform vec3 uSunsetColor;
uniform vec3 uSunsetDirection;
uniform sampler2D uDayGradient;
uniform sampler2D uNightGradient;

out vec4 FragColor;

void main()
{
    vec3 coord = normalize(FragCoord);

    // Lerp the color based on y value of the coord
    // Idk why but the y at the top is negative
    vec2 gradientCoord = vec2(1.0 - clamp(0.0, 1.0, coord.y), 0.5);

    vec3 dayCol = texture(uDayGradient, gradientCoord).rgb;
    vec3 nightCol = texture(uNightGradient, gradientCoord).rgb;

    // Blend between day and night value
    vec3 col = mix(dayCol, nightCol, uSkyDarkness);

    // Calculate the direction multiplier for the sunset color
    // Makes the orange more prominent near the sun
    float directionMultipler = dot(coord, uSunsetDirection);
    directionMultipler += uSunsetCoverage; // Make the sunset cover more than a hemisphere
    directionMultipler /= 1 + uSunsetCoverage; // Remap to 0 to 1 range
    directionMultipler = clamp(directionMultipler, 0.0, 1.0);

    // Mix in the sunset color
    col = mix(col, uSunsetColor, clamp(uSunsetStrength * directionMultipler, 0.0, 1.0));

    // Set the output color
    FragColor = vec4(col, 1.0);
}
