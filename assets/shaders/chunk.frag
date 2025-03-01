#version 330 core

in vec2 TexCoord;
in vec3 Shading;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord) * vec4(Shading, 1.0);

    if (FragColor.a == 0) {
        discard;
    }
}
