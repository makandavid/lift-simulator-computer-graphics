#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>

struct Button
{
    float x, y, w, h;
    bool isFloorBtn = false;
    bool isStopBtn = false;
    bool isDoorBtn = false;
    bool isFanBtn = false;
    int floor = -1;
    int selected = false;

    unsigned texture = 0;
};

class App
{
public:
    GLFWwindow* window = nullptr;
    int w, h;
    float floorHeight;
    bool running = true;

    Renderer renderer;

    // lift
    float liftY;
    bool isOpen = false;
    float doorTimer = 0.0f;
    int liftFloor;
    int targetFloor;
    bool isStopped = false;
    bool isMoving = false;
    bool isFanOn = false;
    bool personInside = false;

    // person
    float personX, personY;
    float personHeight, personWidth;
    int personDir; // -1 left, 1 right, 0 still
    int personFloor;
    bool isDead = false;

    Button buttons[12];

    GLFWcursor* cursorNormal = nullptr;
    GLFWcursor* cursorFan = nullptr;

    unsigned texHallway = 0;
    unsigned texLiftShaft = 0;
    unsigned texLiftDoorClosed = 0;
    unsigned texLiftDoorOpened = 0;
    unsigned texGround = 0;
    unsigned texSky = 0;
    unsigned texPersonWalking = 0;
    unsigned texPersonStanding = 0;
    unsigned texSignature = 0;
    unsigned texYouDied = 0;
    unsigned texReset = 0;

    Button resetButton;

    void init();
    void update(float dt);
    void render();
    void renderPerson(int personDir);
    void reset();
};
