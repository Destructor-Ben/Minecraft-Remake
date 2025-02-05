#version 330 core

layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec2 iTexCoord;

uniform mat4 uTransform;
uniform vec2 uTexCoordPosition;
uniform vec2 uTexCoordScale;

out vec2 TexCoord;

void main()
{
    gl_Position = uTransform * vec4(iPosition, 0, 1);
    TexCoord = uTexCoordPosition + iTexCoord * uTexCoordScale;
}
