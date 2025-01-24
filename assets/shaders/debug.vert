#version 330 core

layout (location = 0) in vec4 iPosition;

uniform mat4 uTransform;

out vec3 Position;

void main()
{
    Position = iPosition.xyz;
    gl_Position = uTransform * iPosition;
}
