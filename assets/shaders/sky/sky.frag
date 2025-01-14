#version 330 core

in vec3 FragCoord;

uniform float uSkyDarkness;
uniform float uSunsetStrength;
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

    vec3 dayCol = texture(uDayGradient, gradientCoord).rgb;
    vec3 nightCol = texture(uNightGradient, gradientCoord).rgb;
    vec4 sunsetCol = texture(uSunsetGradient, gradientCoord);

    // Blend between day and night value
    vec3 col = mix(dayCol, nightCol, uSkyDarkness);

    // Calculate the direction multiplier for the sunset color
    // Makes the orange more prominent near the sun
    float directionMultipler = dot(coord, normalize(vec3(-1, -1.25, 0)));
    directionMultipler = clamp(directionMultipler, 0.0, 1.0);

    // Mix in the sunset value
    col = mix(col, vec3(251.0 / 255.0, 130.0 / 255.0, 9.0 / 255.0), uSunsetStrength * directionMultipler);

    // Set the output color
    FragColor = vec4(col, 1.0);
}
