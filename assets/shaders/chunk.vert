#version 330 core

layout (location = 0) in vec4 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;
layout (location = 3) in vec3 iTint;

uniform mat4 uTransform;

out vec2 TexCoord;
out vec3 Tint;
out vec3 Normal;

void main()
{
    gl_Position = uTransform * iPosition;
    TexCoord = iTexCoord;
    Tint = iTint;
    Normal = iNormal;
}
