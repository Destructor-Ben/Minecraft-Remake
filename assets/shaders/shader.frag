#version 330 core

in vec2 TexCoord;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
   FragColor = mix(texture(uTexture, TexCoord), vec4(TexCoord.x, TexCoord.y, 0.0f, 1.0f), 0.5f);
}
