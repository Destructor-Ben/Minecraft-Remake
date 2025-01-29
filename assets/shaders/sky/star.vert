#version 330 core

layout (location = 0) in vec4 iPosition;
layout (location = 1) in vec2 iTexCoord;
layout (location = 2) in mat4 iTransform;
// Location 6 because the transform actually uses 4 attributes since the max attribute size is a vec4
layout (location = 6) in float iBrightness;
layout (location = 7) in float iTemperature;
layout (location = 8) in float iTwinkleOffset;
layout (location = 9) in float iTextureIndex;

uniform mat4 uTransform;

out vec2 TexCoord;
out float Brightness;
out float Temperature;
out float TwinkleOffset;
out int TextureIndex;

void main()
{
    TexCoord = iTexCoord;
    Brightness = iBrightness;
    Temperature = iTemperature;
    TwinkleOffset = iTwinkleOffset;
    TextureIndex = int(iTextureIndex);

    vec4 position = uTransform * iTransform * iPosition;
    gl_Position = position.xyww;
}
