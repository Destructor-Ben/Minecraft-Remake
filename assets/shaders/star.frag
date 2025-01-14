#version 330 core

in vec2 TexCoord;
in float Brightness;
in float Temperature;

uniform float uSkyDarkness;
uniform sampler2D uTexture;
uniform sampler2D uTemperatureGradient;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord);
    FragColor.rgb *= texture(uTemperatureGradient, vec2(Temperature, 0.5)).rgb; // Temperature
    FragColor.rgb *= 1.1; // Bloom (makes temperaure look less harsh)

    if (FragColor.a == 0) {
        discard;
    }

    FragColor.a = Brightness * 0.5 + 0.5; // Fading in and out

    if (FragColor.a == 0) {
        discard;
    }
}
