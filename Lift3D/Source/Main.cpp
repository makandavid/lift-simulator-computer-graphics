#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Header/Util.h"
#include "../Header/App.h"

static App* gApp = nullptr;

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);  // get current window size

    double centerX = width / 2.0;
    double centerY = height / 2.0;

    static bool firstMouse = true;
    static double lastX = centerX, lastY = centerY;

    if (firstMouse)
    {
        lastX = centerX;
        lastY = centerY;
        firstMouse = false;
        glfwSetCursorPos(window, centerX, centerY);
        return;
    }

    double xoffset = xpos - centerX;
    double yoffset = centerY - ypos; // reversed Y

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    gApp->camera.yaw += xoffset;
    gApp->camera.pitch += yoffset;

    // clamp pitch
    if (gApp->camera.pitch > 89.0f) gApp->camera.pitch = 89.0f;
    if (gApp->camera.pitch < -89.0f) gApp->camera.pitch = -89.0f;

    gApp->camera.update(window);

    // reset cursor to center
    glfwSetCursorPos(window, centerX, centerY);
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
    //GLFWwindow* window = glfwCreateWindow(800, 600, "Lift 3D", NULL, nullptr);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    double last = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double delta = now - last;
        last = now;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        app.processOutsideButtonClick(window);
        app.update(delta);
        app.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        while (glfwGetTime() - now < 1 / 75.0) {}
    }

    glfwTerminate();
    return 0;
}
