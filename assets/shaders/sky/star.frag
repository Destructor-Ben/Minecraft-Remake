#version 330 core

in vec2 TexCoord;
in float Brightness;
in float Temperature;
in float TwinkleSpeed;
in float TwinkleOffset;
flat in int TextureIndex;

uniform float uSkyDarkness;
uniform float uTemperatureStrength;
uniform float uTwinkleStrength;
uniform float uMaxBrightness;
uniform float uTime;
uniform sampler2D uTexture;
uniform sampler2D uTemperatureGradient;

out vec4 FragColor;

const float PI = 3.14159265359;

void main()
{
    // Sample the correct texture
    vec2 texCoord = TexCoord;
    texCoord.x /= 3.0;
    texCoord.x += float(TextureIndex) / 3.0;
    FragColor = texture(uTexture, texCoord);

    // Temperature
    vec3 temperatureCol = texture(uTemperatureGradient, vec2(Temperature, 0.5)).rgb;
    FragColor.rgb = mix(FragColor.rgb, temperatureCol, uTemperatureStrength);

    // Fading in and out
    // Normally we would think to just add brightness, but it means the stars are visibile during the day
    // So subtract the max star brightness, which also makes the less bright stars visibily less bright
    FragColor.a *= uMaxBrightness * (uSkyDarkness - 1 + Brightness);

    // Twinkling
    // Remap uTime to make a second cycle from 101
    FragColor.a *= mix(uTwinkleStrength, 1.0f, cos(uTime * 2 * PI * TwinkleSpeed + TwinkleOffset) * 0.5 + 0.5);

    // Clamp to be safe
    FragColor.a = clamp(FragColor.a, 0.0, uMaxBrightness);

    if (FragColor.a == 0) {
        discard;
    }
}
