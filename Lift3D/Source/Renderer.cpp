#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Header/Renderer.h"
#include "../Header/Util.h"

float cubeVertices[] = {
    // positions          
    // Front face
    -0.5f, -0.5f,  0.5f,  // bottom-left
     0.5f, -0.5f,  0.5f,  // bottom-right
     0.5f,  0.5f,  0.5f,  // top-right
     0.5f,  0.5f,  0.5f,  // top-right
    -0.5f,  0.5f,  0.5f,  // top-left
    -0.5f, -0.5f,  0.5f,  // bottom-left

    // Back face
    -0.5f, -0.5f, -0.5f,  // bottom-left
    -0.5f,  0.5f, -0.5f,  // top-left
     0.5f,  0.5f, -0.5f,  // top-right
     0.5f,  0.5f, -0.5f,  // top-right
     0.5f, -0.5f, -0.5f,  // bottom-right
    -0.5f, -0.5f, -0.5f,  // bottom-left

    // Left face
    -0.5f,  0.5f,  0.5f,  // top-right
    -0.5f,  0.5f, -0.5f,  // top-left
    -0.5f, -0.5f, -0.5f,  // bottom-left
    -0.5f, -0.5f, -0.5f,  // bottom-left
    -0.5f, -0.5f,  0.5f,  // bottom-right
    -0.5f,  0.5f,  0.5f,  // top-right

    // Right face
     0.5f,  0.5f,  0.5f,  // top-left
     0.5f, -0.5f,  0.5f,  // bottom-left
     0.5f, -0.5f, -0.5f,  // bottom-right
     0.5f, -0.5f, -0.5f,  // bottom-right
     0.5f,  0.5f, -0.5f,  // top-right
     0.5f,  0.5f,  0.5f,  // top-left

     // Top face
     -0.5f,  0.5f, -0.5f,  // top-left
     -0.5f,  0.5f,  0.5f,  // bottom-left
      0.5f,  0.5f,  0.5f,  // bottom-right
      0.5f,  0.5f,  0.5f,  // bottom-right
      0.5f,  0.5f, -0.5f,  // top-right
     -0.5f,  0.5f, -0.5f,  // top-left

     // Bottom face
     -0.5f, -0.5f, -0.5f,  // top-left
      0.5f, -0.5f, -0.5f,  // top-right
      0.5f, -0.5f,  0.5f,  // bottom-right
      0.5f, -0.5f,  0.5f,  // bottom-right
     -0.5f, -0.5f,  0.5f,  // bottom-left
     -0.5f, -0.5f, -0.5f   // top-left
};


void Renderer::init()
{
    shader = createShader("Shaders/basic.vert", "Shaders/basic.frag");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(cubeVertices),
        cubeVertices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Renderer::drawCube(glm::vec3 pos, glm::vec3 size, glm::vec3 color, Camera& cam)
{
    glUseProgram(shader);

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

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
