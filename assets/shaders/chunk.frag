#version 330 core

in vec2 TexCoord;
in vec3 Tint;
in vec3 Normal;

uniform sampler2D uTexture;
uniform vec3 uAmbientLight;
uniform vec3 uDirToSun;
uniform vec3 uDirToMoon;
uniform vec3 uSunLight;
uniform vec3 uMoonLight;

out vec4 FragColor;

void main()
{
    // Base color
    FragColor = texture(uTexture, TexCoord) * vec4(Tint, 1.0);

    if (FragColor.a == 0) {
        discard;
    }

    // Calculate lighting
    // TODO: this can be done easily in the vertex shader for directional lights since the lighting is consistent across faces with flat shading
    // - Consider doing point/spot lights in vertex shader too and just interpolating for the fragment shader to make it more performant
    //   - make this an option in settings
    // TODO: apparently normal needs to be transformed in the vertex shader/set as a uniform since it is in model space and needs to be in world space
    vec3 lighting = uAmbientLight;
    lighting += clamp(dot(Normal, uDirToSun), 0.0, 1.0) * uSunLight;
    lighting += clamp(dot(Normal, uDirToMoon), 0.0, 1.0) * uMoonLight;
    FragColor.rgb *= lighting;
}
