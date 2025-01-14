#version 330 core

layout (location = 0) in vec4 iPosition;

uniform mat4 uTransform;

out vec3 FragCoord;

void main()
{
    FragCoord = vec3(iPosition);
    vec4 position = uTransform * iPosition;

    // This trick makes the depth value always 1, since z is the depth
    // Perspective division occurs after this were z = z / w
    // So if z = w then w / w = 1 so the depth is 1
    // Allows us to draw after the scene
    gl_Position = position.xyww;
}
