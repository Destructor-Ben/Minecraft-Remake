#version 330 core

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, TexCoord);
    FragColor *= uColor;

    if (FragColor.a == 0) {
        discard;
    }
}
