#version 330 core

layout (location = 0) in vec4 iPosition;

uniform mat4 uTransform;

out vec3 TexCoord;

void main()
{
    gl_Position = uTransform * iPosition;
    TexCoord = vec4(iPosition);
}
