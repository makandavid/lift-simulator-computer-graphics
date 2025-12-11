#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform vec4 uColor;
uniform sampler2D uTex;
uniform bool uUseTexture;

void main()
{
    if(uUseTexture)
        fragColor = texture(uTex, texCoord);
    else
        fragColor = uColor;
}
