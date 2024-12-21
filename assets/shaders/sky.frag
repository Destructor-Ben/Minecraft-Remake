#version 330 core

in vec3 TexCoord;

uniform samplerCube uSkyBox;

out vec4 FragColor;

void main()
{
    FragColor = texture(uSkyBox, TexCoord);
}
