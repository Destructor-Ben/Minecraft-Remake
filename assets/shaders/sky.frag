#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

void main()
{
    FragColor = vec4(TexCoord, 1.0);
}
