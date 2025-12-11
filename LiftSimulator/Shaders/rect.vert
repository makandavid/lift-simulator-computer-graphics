#version 330 core

layout(location = 0) in vec2 aPos;

uniform mat4 uModel;
uniform vec2 uScreen;
uniform bool uFlipX;

out vec2 texCoord;

void main()
{
    vec4 world = uModel * vec4(aPos, 0.0, 1.0);

    float x = (world.x / uScreen.x) * 2.0 - 1.0;
    float y = 1.0 - (world.y / uScreen.y) * 2.0;

    gl_Position = vec4(x, y, 0.0, 1.0);

    texCoord = vec2(aPos.x, 1.0 - aPos.y);

    if (uFlipX)
        texCoord.x = 1.0 - texCoord.x;

}
