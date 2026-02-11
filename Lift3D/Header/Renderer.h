#pragma once
#include "Camera.h"

class Renderer
{
    unsigned vao, vbo;
    unsigned shader;


public:
    GLuint texWall;
    GLuint texFloor;
    GLuint texShaft;
    GLuint texElevator;
    GLuint texDoor;
    GLuint texGrass;

    GLuint texBtn1;
    GLuint texBtn2;
    GLuint texBtn3;
    GLuint texBtn4;
    GLuint texBtn5;
    GLuint texBtn6;
    GLuint texBtnPR;
    GLuint texBtnSU;
    GLuint texBtnOpen;
    GLuint texBtnClose;
    GLuint texBtnStop;
    GLuint texBtnFan;

    void init();
    void drawCube(glm::vec3 pos, glm::vec3 size, glm::vec3 color, GLuint texture, Camera& cam);
};
