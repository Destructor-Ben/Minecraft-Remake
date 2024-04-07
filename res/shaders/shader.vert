#version 330 core

layout (location = 0) in vec4 iPosition;
layout (location = 1) in vec2 iTexCoord;

uniform mat4 uTransform;

out vec2 TexCoord;

void main()
{
    gl_Position = uTransform * iPosition;
    TexCoord = iTexCoord;
}
