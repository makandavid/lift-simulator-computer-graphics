#include "../Header/Renderer.h"
#include "../Header/Util.h"

static const float quad[12] = {
    0,0, 1,0, 1,1,
    0,0, 1,1, 0,1
};

void Renderer::init(int screenW, int screenH)
{
    shader = createShader("Shaders/rect.vert", "Shaders/rect.frag");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glUseProgram(shader);
    glUniform2f(glGetUniformLocation(shader, "uScreen"), (float)screenW, (float)screenH);
}

void Renderer::drawRect(float x, float y, float w, float h, Color c)
{
    glUseProgram(shader);
    glBindVertexArray(vao);

    float model[16] = {
        w,0,0,0,
        0,h,0,0,
        0,0,1,0,
        x,y,0,1
    };

    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, model);
    glUniform4f(glGetUniformLocation(shader, "uColor"), c.r, c.g, c.b, c.a);
    glUniform1i(glGetUniformLocation(shader, "uUseTexture"), 0);


    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawTexturedRect(float x, float y, float w, float h, unsigned texture, bool flip)
{
    glUseProgram(shader);
    glBindVertexArray(vao);

    float model[16] = {
        w,0,0,0,
        0,h,0,0,
        0,0,1,0,
        x,y,0,1
    };

    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, model);
    glUniform1i(glGetUniformLocation(shader, "uUseTexture"), 1);
    glUniform1i(glGetUniformLocation(shader, "uFlipX"), flip);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader, "uTex"), 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

