#version 330 core

in vec2 TexCoord;

uniform float uSkyDarkness;
uniform float uMinBrightness;
uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord);
    FragColor.a *= mix(uMinBrightness, 1, uSkyDarkness);

    if (FragColor.a == 0) {
        discard;
    }
}
