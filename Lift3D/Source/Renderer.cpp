#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Header/Renderer.h"
#include "../Header/Util.h"

float cubeVertices[] = {
    // positions           normals           texcoords
    // Front (+Z)
    -0.5f,-0.5f, 0.5f,     0,0,1,            0,0,
     0.5f,-0.5f, 0.5f,     0,0,1,            1,0,
     0.5f, 0.5f, 0.5f,     0,0,1,            1,1,
     0.5f, 0.5f, 0.5f,     0,0,1,            1,1,
    -0.5f, 0.5f, 0.5f,     0,0,1,            0,1,
    -0.5f,-0.5f, 0.5f,     0,0,1,            0,0,

    // Back (-Z)
    -0.5f,-0.5f,-0.5f,     0,0,-1,           1,0,
    -0.5f, 0.5f,-0.5f,     0,0,-1,           1,1,
     0.5f, 0.5f,-0.5f,     0,0,-1,           0,1,
     0.5f, 0.5f,-0.5f,     0,0,-1,           0,1,
     0.5f,-0.5f,-0.5f,     0,0,-1,           0,0,
    -0.5f,-0.5f,-0.5f,     0,0,-1,           1,0,

    // Left (-X)
    -0.5f, 0.5f, 0.5f,    -1,0,0,            1,1,
    -0.5f, 0.5f,-0.5f,    -1,0,0,            0,1,
    -0.5f,-0.5f,-0.5f,    -1,0,0,            0,0,
    -0.5f,-0.5f,-0.5f,    -1,0,0,            0,0,
    -0.5f,-0.5f, 0.5f,    -1,0,0,            1,0,
    -0.5f, 0.5f, 0.5f,    -1,0,0,            1,1,

    // Right (+X)
     0.5f, 0.5f, 0.5f,     1,0,0,            0,1,
     0.5f,-0.5f, 0.5f,     1,0,0,            0,0,
     0.5f,-0.5f,-0.5f,     1,0,0,            1,0,
     0.5f,-0.5f,-0.5f,     1,0,0,            1,0,
     0.5f, 0.5f,-0.5f,     1,0,0,            1,1,
     0.5f, 0.5f, 0.5f,     1,0,0,            0,1,

     // Top (+Y)
     -0.5f, 0.5f,-0.5f,     0,1,0,            0,1,
     -0.5f, 0.5f, 0.5f,     0,1,0,            0,0,
      0.5f, 0.5f, 0.5f,     0,1,0,            1,0,
      0.5f, 0.5f, 0.5f,     0,1,0,            1,0,
      0.5f, 0.5f,-0.5f,     0,1,0,            1,1,
     -0.5f, 0.5f,-0.5f,     0,1,0,            0,1,

     // Bottom (-Y)
     -0.5f,-0.5f,-0.5f,     0,-1,0,           1,1,
      0.5f,-0.5f,-0.5f,     0,-1,0,           0,1,
      0.5f,-0.5f, 0.5f,     0,-1,0,           0,0,
      0.5f,-0.5f, 0.5f,     0,-1,0,           0,0,
     -0.5f,-0.5f, 0.5f,     0,-1,0,           1,0,
     -0.5f,-0.5f,-0.5f,     0,-1,0,           1,1
};

Renderer::Renderer()
    : shader("Shaders/basic.vert", "Shaders/basic.frag")
{
}

void Renderer::init()
{
    //shader = createShader("Shaders/basic.vert", "Shaders/basic.frag");

    preprocessTexture(texWall, "Resources/textures/wall.jpg");
    preprocessTexture(texFloor, "Resources/textures/floor.jpg");
    preprocessTexture(texShaft, "Resources/textures/shaft.jpg");
    preprocessTexture(texElevator, "Resources/textures/elevator.jpg");
    preprocessTexture(texDoor, "Resources/textures/door.jpg");
    preprocessTexture(texGrass, "Resources/textures/grass.jpg");
    preprocessTexture(texBtn1, "Resources/textures/btn_1.png");
    preprocessTexture(texBtn2, "Resources/textures/btn_2.png");
    preprocessTexture(texBtn3, "Resources/textures/btn_3.png");
    preprocessTexture(texBtn4, "Resources/textures/btn_4.png");
    preprocessTexture(texBtn5, "Resources/textures/btn_5.png");
    preprocessTexture(texBtn6, "Resources/textures/btn_6.png");
    preprocessTexture(texBtnSU, "Resources/textures/btn_SU.png");
    preprocessTexture(texBtnPR, "Resources/textures/btn_PR.png");
    preprocessTexture(texBtnFan, "Resources/textures/btn_fan.png");
    preprocessTexture(texBtnOpen, "Resources/textures/btn_open.png");
    preprocessTexture(texBtnClose, "Resources/textures/btn_close.png");
    preprocessTexture(texBtnStop, "Resources/textures/btn_stop.png");


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(cubeVertices),
        cubeVertices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



}

void Renderer::drawCube(glm::vec3 pos,
    glm::vec3 size,
    glm::vec3 color,
    GLuint texture,
    Camera& cam)
{
    shader.use();

    shader.setVec3("lightPos", { 0.0f, 50.0f, 0.0f });
    shader.setVec3("viewPos", cam.position);
    shader.setVec3("lightColor", { 1.0f, 1.0f, 1.0f });
    shader.setVec3("objectColor", { 1.0f, 1.0f, 1.0f });

    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::scale(model, size);

    shader.setMat4("model", model);
    shader.setMat4("view", cam.getView());
    shader.setMat4("proj", cam.getProjection());

    shader.setVec3("uColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setInt("tex", 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void Renderer::drawModel(Model& model, glm::vec3 position, float scale, Camera& camera, glm::vec3 rotation)
{
    shader.use();

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);

    if (rotation.x != 0.0f)
        modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotation.y != 0.0f)
        modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotation.z != 0.0f)
        modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    modelMat = glm::scale(modelMat, glm::vec3(scale));

    shader.setMat4("model", modelMat);
    shader.setMat4("view", camera.getView());
    shader.setMat4("projection", camera.getProjection());

    model.Draw(shader);
}

void Renderer::clearLights()
{
    pointLights.clear();
}

void Renderer::addLight(glm::vec3 pos, glm::vec3 color)
{
    pointLights.push_back({ pos, color });
}

void Renderer::uploadLights(Camera& cam)
{
    shader.use();

    int count = (int)pointLights.size();
    if (count > 32) count = 32;

    shader.setInt("numLights", count);
    shader.setVec3("viewPos", cam.position);

    for (int i = 0; i < count; i++)
    {
        std::string base = "lights[" + std::to_string(i) + "]";
        shader.setVec3(base + ".position", pointLights[i].position);
        shader.setVec3(base + ".color", pointLights[i].color);
    }
}
