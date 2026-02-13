#pragma once
#include "Camera.h"
#include "Model.h"

class Renderer
{
    unsigned vao, vbo;
    Shader shader;


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

    struct PointLight
    {
        glm::vec3 position;
        glm::vec3 color;
    };
    std::vector<PointLight> pointLights;

    Renderer();
    void init();
    void drawCube(glm::vec3 pos, glm::vec3 size, glm::vec3 color, GLuint texture, Camera& cam);
    void drawModel(Model& model, glm::vec3 position, float scale, Camera& camera, glm::vec3 rotation);

    void clearLights();
    void addLight(glm::vec3 pos, glm::vec3 color);
    void uploadLights(Camera& cam);
};
