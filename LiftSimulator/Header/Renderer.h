#pragma once
#include <GL/glew.h>
#include <string>

struct Color { float r, g, b, a; };

class Renderer
{
    GLuint vao, vbo;
    GLuint shader;

public:
    void init(int screenW, int screenH);
    void drawRect(float x, float y, float w, float h, Color color);
    void drawTexturedRect(float x, float y, float w, float h, unsigned texture, bool flip = false);
};
