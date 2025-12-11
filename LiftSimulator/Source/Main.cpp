#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Header/App.h"
#include "../Header/Input.h"
#include "../Header/Util.h"
#include <thread>
#include <iostream>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    App app{};
    app.w = mode->width;
    app.h = mode->height;

    app.window = glfwCreateWindow(app.w, app.h, "Lift", monitor, nullptr);
    if (!app.window) return endProgram("Failed window");

    glfwMakeContextCurrent(app.window);

    if (glewInit() != GLEW_OK) return endProgram("GLEW init failed");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    registerInput(app.window, &app);
    app.init();

    double last = glfwGetTime();
    const double dt = 1.0 / 75.0;

    while (app.running)
    {
        double now = glfwGetTime();
        double delta = now - last;
        last = now;

        glfwPollEvents();
        app.update((float)delta);
        app.render();
        glfwSwapBuffers(app.window);

        while (glfwGetTime() - now < 1 / 75.0) {}
    }

    glfwTerminate();
    return 0;
}
