#version 330 core

layout (location = 0) in vec4 Position;

uniform mat4 uTransform;

void main()
{
    gl_Position = uTransform * Position;
}
