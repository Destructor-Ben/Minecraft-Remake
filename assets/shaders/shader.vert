#version 330 core

layout (location = 0) in vec4 iPosition;
layout (location = 1) in vec2 iTexCoord;
layout (location = 2) in vec4 iShading;

uniform mat4 uTransform;

layout (location = 0) out vec2 TexCoord;
layout (location = 1) out vec4 Shading;

void main()
{
    gl_Position = uTransform * iPosition;
    TexCoord = iTexCoord;
    Shading = iShading;
}
