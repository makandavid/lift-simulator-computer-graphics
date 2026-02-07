#pragma once
#include "Camera.h"

class Renderer
{
    unsigned vao, vbo;
    unsigned shader;

public:
    void init();
    void drawCube(glm::vec3 pos, glm::vec3 size, glm::vec3 color, Camera& cam);
};
