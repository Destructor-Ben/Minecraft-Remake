#version 330 core

layout (location = 0) in vec2 TexCoord;
layout (location = 1) in vec4 Shading;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord) * Shading;
}
