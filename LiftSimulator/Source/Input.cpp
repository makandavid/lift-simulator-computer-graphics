#include "../Header/App.h"
#include "../Header/Input.h"
#include <iostream>

static App* app;

static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    double mx, my;
    glfwGetCursorPos(app->window, &mx, &my);

    if (mx >= app->resetButton.x && mx <= app->resetButton.x + app->resetButton.w &&
        my >= app->resetButton.y && my <= app->resetButton.y + app->resetButton.h && app->isDead)
    {
        app->reset();
    }

    for (auto& b : app->buttons)
    {
        if (app->personX < app->w * 0.85f) 
            return;
        if (mx >= b.x && mx <= b.x + b.w &&
            my >= b.y && my <= b.y + b.h)
        {
            if (b.isFloorBtn)
            {
                app->isStopped = false;
                b.selected = true;
            }

            if (b.isDoorBtn && b.texture == app->buttons[8].texture && !app->isMoving)
            {
                app->isOpen = true;
                app->doorTimer = 5.0f;
            }

            if (b.isDoorBtn && b.texture == app->buttons[9].texture)
            {
                app->isOpen = false;
                app->doorTimer = 0;
            }

            if (b.isStopBtn)
            {
                app->isStopped = true;
                app->targetFloor = -1;
                for (int i = 0; i < 8; i++)
                    app->buttons[i].selected = false;
            }

            if (b.isFanBtn)
            {
                app->isFanOn = !app->isFanOn;

                if (app->isFanOn)
                    glfwSetCursor(app->window, app->cursorFan);
                else
                    glfwSetCursor(app->window, app->cursorNormal);
            }
        }
    }
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_A) app->personDir = -1;
        if (key == GLFW_KEY_D) app->personDir = 1;
        if (key == GLFW_KEY_C && app->personX >= app->w * 0.85f - 30) 
        {
            app->buttons[app->personFloor].selected = true;
            app->isOpen = true;
        }
    }
    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_A || key == GLFW_KEY_D) app->personDir = 0;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        app->running = false;
}

void registerInput(GLFWwindow* window, App* appPtr)
{
    app = appPtr;
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
}
