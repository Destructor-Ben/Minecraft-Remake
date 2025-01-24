#version 330 core

in vec3 Position;

uniform vec3 uColor;
uniform bool uDrawCircle;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uColor, 1.0);

    if (uDrawCircle && length(Position) > 0.5) {
        discard;
    }
}
