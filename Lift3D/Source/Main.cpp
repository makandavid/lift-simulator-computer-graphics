#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Header/Util.h"
#include "../Header/App.h"

static App* gApp = nullptr;

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;
    static float sensitivity = 0.1f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: top-left = (0,0)
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    gApp->camera.yaw += xoffset; // yaw is rotation around **global Y axis**
    gApp->camera.pitch += yoffset; // pitch is rotation around camera’s local X axis

    // clamp pitch
    if (gApp->camera.pitch > 89.0f) gApp->camera.pitch = 89.0f;
    if (gApp->camera.pitch < -89.0f) gApp->camera.pitch = -89.0f;

    gApp->camera.update(window);
}


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Lift 3D", monitor, nullptr);
    if (!window) return endProgram("Failed window");

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) return endProgram("GLEW init failed");

    glEnable(GL_DEPTH_TEST);

    App app{};
    app.window = window;
    gApp = &app;

    app.camera.w = mode->width;
    app.camera.h = mode->height;
    app.init();

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double last = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double delta = now - last;
        last = now;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        app.camera.processInput(window, delta);

        app.update(delta);
        app.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        while (glfwGetTime() - now < 1 / 75.0) {}
    }

    glfwTerminate();
    return 0;
}
