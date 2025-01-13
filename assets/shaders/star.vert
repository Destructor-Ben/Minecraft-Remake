#version 330 core

layout (location = 0) in vec4 iPosition;
layout (location = 1) in vec2 iTexCoord;
layout (location = 2) in mat4 iTransform;
// Location 6 because the transform actually uses 4 attributes since the max attribute size is a vec4
layout (location = 6) in float iTemperature;

uniform mat4 uTransform;

out vec2 TexCoord;
out float Temperature;

void main()
{
    TexCoord = iTexCoord;
    Temperature = iTemperature;

    vec4 position = uTransform * iTransform * iPosition;

    // This trick makes the depth value always 1, since z is the depth
    // Perspective division occurs after this were z = z / w
    // So if z = w then w / w = 1 so the depth is 1
    // Allows us to draw after the scene
    gl_Position = position.xyww;
}
