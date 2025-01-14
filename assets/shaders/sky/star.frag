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

    if (FragColor.a == 0) {
        discard;
    }

    // Temperature
    vec3 temperatureCol = texture(uTemperatureGradient, vec2(Temperature, 0.5)).rgb;
    FragColor.rgb = mix(FragColor.rgb, temperatureCol, 0.25);

    // Fading in and out
    FragColor.a = mix(0.0, 0.75, uSkyDarkness - 1 + Brightness);
    FragColor.a = clamp(1.0, 0.0, FragColor.a);

    if (FragColor.a == 0) {
        discard;
    }
}
