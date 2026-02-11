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



void Renderer::init()
{
    shader = createShader("Shaders/basic.vert", "Shaders/basic.frag");

    preprocessTexture(texWall, "Resources/wall.jpg");
    preprocessTexture(texFloor, "Resources/floor.jpg");
    preprocessTexture(texShaft, "Resources/shaft.jpg");
    preprocessTexture(texElevator, "Resources/elevator.jpg");
    preprocessTexture(texDoor, "Resources/door.jpg");
    preprocessTexture(texGrass, "Resources/grass.jpg");
    preprocessTexture(texBtn1, "Resources/btn_1.png");
    preprocessTexture(texBtn2, "Resources/btn_2.png");
    preprocessTexture(texBtn3, "Resources/btn_3.png");
    preprocessTexture(texBtn4, "Resources/btn_4.png");
    preprocessTexture(texBtn5, "Resources/btn_5.png");
    preprocessTexture(texBtn6, "Resources/btn_6.png");
    preprocessTexture(texBtnSU, "Resources/btn_SU.png");
    preprocessTexture(texBtnPR, "Resources/btn_PR.png");
    preprocessTexture(texBtnFan, "Resources/btn_fan.png");
    preprocessTexture(texBtnOpen, "Resources/btn_open.png");
    preprocessTexture(texBtnClose, "Resources/btn_close.png");
    preprocessTexture(texBtnStop, "Resources/btn_stop.png");


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

void Renderer::drawCube(glm::vec3 pos, glm::vec3 size, glm::vec3 color, GLuint texture, Camera& cam)
{
    glUseProgram(shader);

    glUniform3f(glGetUniformLocation(shader, "lightPos"),
        0.0f, 50.0f, 0.0f);

    glUniform3f(glGetUniformLocation(shader, "viewPos"),
        cam.position.x,
        cam.position.y,
        cam.position.z);

    glUniform3f(glGetUniformLocation(shader, "lightColor"),
        1.0f, 1.0f, 1.0f);

    glUniform3f(glGetUniformLocation(shader, "objectColor"),
        1.0f, 1.0f, 1.0f);


    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

    model = glm::scale(model, size);

    auto view = cam.getView();
    auto proj = cam.getProjection();

    glUniformMatrix4fv(
        glGetUniformLocation(shader, "model"),
        1, GL_FALSE, &model[0][0]);

    glUniformMatrix4fv(
        glGetUniformLocation(shader, "view"),
        1, GL_FALSE, &view[0][0]);

    glUniformMatrix4fv(
        glGetUniformLocation(shader, "proj"),
        1, GL_FALSE, &proj[0][0]);

    glUniform3fv(
        glGetUniformLocation(shader, "uColor"), 
        1, &color[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(glGetUniformLocation(shader, "tex"), 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
