#version 330 core

in vec3 FragCoord;

uniform float uTime;
uniform sampler2D uDayGradient;
uniform sampler2D uNightGradient;

out vec4 FragColor;

void main()
{
    vec3 coord = normalize(FragCoord);

    // Lerp the color based on y value of the coord
    // TODO: why a negative in the y?
    vec2 gradientCoord = vec2(-coord.y * 0.5 + 0.5, 0.5);
    vec3 dayCol = texture(uDayGradient, gradientCoord).rgb;
    vec3 nightCol = texture(uNightGradient, gradientCoord).rgb;
    vec3 col = mix(dayCol, nightCol, uTime);

    FragColor = vec4(col, 1.0);
}
