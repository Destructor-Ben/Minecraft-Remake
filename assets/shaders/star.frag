#version 330 core

in vec2 TexCoord;
in float Temperature;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord);
    // TODO: proper temperature
    FragColor.r = Temperature;

    if (FragColor.a == 0) {
        discard;
    }
}
