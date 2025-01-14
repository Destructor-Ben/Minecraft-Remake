#version 330 core

in vec2 TexCoord;
in float Brightness;
in float Temperature;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord);
    // TODO: proper rendering
    FragColor.r = Brightness;
    FragColor.b = Temperature;

    if (FragColor.a == 0) {
        discard;
    }
}
