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
    // TODO: maybe rotate the gradient a little
    vec3 sunsetCol = texture(uSunsetGradient, gradientCoord).rgb;

    // Blend between day and night value
    vec3 col = mix(dayCol, nightCol, uSkyDarkness);

    // Mix in the sunset value
    col = mix(col, sunsetCol, uSunsetStrength);

    // Set the output color
    FragColor = vec4(col, 1.0);
}
