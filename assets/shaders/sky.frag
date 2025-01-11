#version 330 core

in vec3 FragCoord;

out vec4 FragColor;

void main()
{
    vec3 coord = normalize(FragCoord);

    // Lerp the color based on y value of the coord
    // TODO: make these uniforms
    vec3 dayColor = vec3(123.0 / 256.0, 186.0 / 256.0, 229.0 / 256.0);
    vec3 nightColor = vec3(4.0 / 256.0, 5.0 / 256.0, 36.0 / 256.0);
    // TODO: why is y negative?
    // TODO: multilerp with multiple values
    vec3 col = mix(dayColor, nightColor, -coord.y * 0.5 + 0.5);

    FragColor = vec4(col, 1.0);
}
