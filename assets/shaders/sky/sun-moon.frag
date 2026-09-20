#version 330 core

in vec2 TexCoord;

uniform float uSkyDarkness;
uniform float uMinBrightness;
uniform float uGlowSize;
uniform float uGlowStrength;
uniform vec3 uGlowColor;
uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord);
    FragColor.a *= mix(uMinBrightness, 1, uSkyDarkness);

    if (FragColor.a == 0.0) {
        float glowDst = length((TexCoord - vec2(0.5)) / uGlowSize);
        float glowStrength = clamp(exp(-glowDst) * (1 - glowDst), 0.0, 1.0);

        FragColor.rgb = uGlowColor;
        FragColor.a = glowStrength * uGlowStrength;
    }

    if (FragColor.a == 0.0) {
        discard;
    }
}
