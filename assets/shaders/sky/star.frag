#version 330 core

in vec2 TexCoord;
in float Brightness;
in float Temperature;

uniform float uSkyDarkness;
uniform float uTemperatureStrength;
uniform float uMaxBrightness;
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
    FragColor.rgb = mix(FragColor.rgb, temperatureCol, uTemperatureStrength);

    // Fading in and out
    // Normally we would think to just add brightness, but it means the stars are visibile during the day
    // So subtract the max star brightness, which also makes the less bright stars visibily less bright
    FragColor.a = mix(0.0, uMaxBrightness, uSkyDarkness - 1 + Brightness);
    FragColor.a = clamp(FragColor.a, 0.0, uMaxBrightness);

    if (FragColor.a == 0) {
        discard;
    }
}
