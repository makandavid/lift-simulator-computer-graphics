#include "../Header/App.h"
#include <cmath>
#include <iostream>
#include "../Header/Util.h"

void App::init()
{
    renderer.init(w, h);
    floorHeight = h / 8.0f;

    cursorNormal = loadImageToCursor("Resources/cursor_normal.png");
    cursorFan = loadImageToCursor("Resources/cursor_colored.png");
    glfwSetCursor(window, cursorNormal);

    const int BTN_COLS = 2;
    const int BTN_ROWS = 6;      // 12 buttons total
    const float BTN_W = 100.0f;
    const float BTN_H = 100.0f;
    const float BTN_START_X = 30.0f;
    const float BTN_MARGIN_BOTTOM = 330.0f;
    const float BTN_GAP_X = 15.0f;
    const float BTN_GAP_Y = 15.0f;

    const char* buttonTextures[12] =
    {
        "Resources/btn_SU.png",
        "Resources/btn_PR.png",
        "Resources/btn_1.png",
        "Resources/btn_2.png",
        "Resources/btn_3.png",
        "Resources/btn_4.png",
        "Resources/btn_5.png",
        "Resources/btn_6.png",
        "Resources/btn_open.png",
        "Resources/btn_close.png",
        "Resources/btn_stop.png",
        "Resources/btn_fan.png"
    };

    preprocessTexture(texHallway, "Resources/hallway.png");
    preprocessTexture(texLiftShaft, "Resources/lift_shaft.png");
    preprocessTexture(texLiftDoorClosed, "Resources/lift_door_closed.png");
    preprocessTexture(texLiftDoorOpened, "Resources/lift_door_opened.png");
    preprocessTexture(texGround, "Resources/ground.png");
    preprocessTexture(texSky, "Resources/sky.png");
    preprocessTexture(texPersonWalking, "Resources/person_walking.png");
    preprocessTexture(texPersonStanding, "Resources/person_standing.png");
    preprocessTexture(texSignature, "Resources/signature.png");
    preprocessTexture(texYouDied, "Resources/you_died.png");
    preprocessTexture(texReset, "Resources/reset.png");

    int index = 0;
    for (int r = 0; r < BTN_ROWS; r++)
    {
        for (int c = 0; c < BTN_COLS; c++)
        {
            Button& b = buttons[index];

            b.x = BTN_START_X + c * (BTN_W + BTN_GAP_X);
            b.y = h - BTN_MARGIN_BOTTOM - BTN_H - r * (BTN_H + BTN_GAP_Y);
            b.w = BTN_W;
            b.h = BTN_H;

            b.isFloorBtn = false;
            b.isStopBtn = false;
            b.isDoorBtn = false;
            b.isFanBtn = false;
            b.selected = false;
            b.floor = -1;

            if (index < 8)
            {
                // first 8 buttons are floors
                b.isFloorBtn = true;
                b.floor = index;   // 0..7
            }
            else
            {
                // last 4 buttons
                switch (index)
                {
                case 8:  // open door
                    b.isDoorBtn = true;
                    break;
                case 9:  // close door
                    b.isDoorBtn = true;
                    break;
                case 10: // stop
                    b.isStopBtn = true;
                    break;
                case 11: // fan
                    b.isFanBtn = true;
                    break;
                }
            }

            preprocessTexture(b.texture, buttonTextures[index]);

            index++;
        }
    }

    liftY = h - floorHeight * 3; // start on floor 1
    targetFloor = -1;
    liftFloor = 2;

    personHeight = 90; personWidth = 30;
    personX = w * 0.3f; personY = h - floorHeight - personHeight; // ground floor
    personFloor = 1;
    personDir = 0;

    resetButton = { w * 0.5f - 221, h * 0.5f + 80, 443, 150 };
}

void App::update(float dt)
{
    if (isDead)
        return;  // freeze simulation until reset

    float outsideDoorX = w * 0.85f - personWidth; // outside edge
    float liftRightEdgeX = w * 0.85f + personWidth; // lift right edge
    float insideDoorX = w * 0.85f;      // cabin interior plane

    bool inDoorway =
        personFloor == liftFloor &&
        personX > outsideDoorX &&
        personX < insideDoorX;


    if (isOpen)
    {
        doorTimer -= dt;
        if (doorTimer <= 0)
        {
            doorTimer = 0;
            isOpen = false;
            if (inDoorway)
            {
                isDead = true;
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; i++) 
        {
            if (buttons[i].selected) 
            {
                targetFloor = buttons[i].floor;
                break;
            }
        }
    }

    // lift movement
    if (!isStopped && targetFloor >= 0)
    {
        float speed = 300;
        float targetY = h - floorHeight * targetFloor - floorHeight;
        if (fabs(liftY - targetY) < 2)
        {
            liftY = targetY;
            liftFloor = targetFloor;
            if (personX - personWidth >= outsideDoorX) {
                personY = targetY + floorHeight - personHeight;
                personFloor = targetFloor;
            }
            isOpen = true;
            isMoving = false;
            doorTimer = 5.0f;
            buttons[targetFloor].selected = false;
            targetFloor = -1;
            isFanOn = false;
            glfwSetCursor(window, cursorNormal);
        }
        else {
            liftY += (liftY < targetY ? speed : -speed) * dt;
            liftFloor = targetFloor;
            if (personX - personWidth >= outsideDoorX) {
                personY += (personY < targetY + floorHeight - personHeight ? speed : -speed) * dt;
                personFloor = targetFloor;
            }
            isMoving = true;

        }
    }

    // person movement
    float pspeed = 200;
    personX += personDir * pspeed * dt;
    if (!personInside && isOpen && personFloor == liftFloor && personX > outsideDoorX)
        personInside = true;

    if (personInside && isOpen && personFloor == liftFloor && personX < outsideDoorX)
        personInside = false;

    if (personX < 0) 
        personX = 0;
    if (personFloor != 1 && personX < w * 0.5f) 
        personX = w * 0.5; // building limit when not on the ground floor

    if (personY != h - floorHeight * personFloor - personHeight && personX < insideDoorX) 
        personX = insideDoorX; // left edge of inside of the lift when moving

    if (personFloor == liftFloor && personX > liftRightEdgeX)  
        personX = liftRightEdgeX; // right edge of the lift
    if (personFloor != liftFloor && personX > outsideDoorX) 
        personX = outsideDoorX; // left edge of outside of the lift when not on the same floor

    if (!isOpen && personInside && personX < insideDoorX) 
        personX = insideDoorX; // inside of the lift when the door is closed
    if (!isOpen && !personInside && personX > outsideDoorX) 
        personX = outsideDoorX; // outside of the lift when the door is closed

}

void App::render()
{
    glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sky
    renderer.drawTexturedRect(0, 0, w * 0.5f, h, texSky);

    // panel
    renderer.drawRect(0, 50, w * 0.15f, h * 0.7f, { 0.4f, 0.4f, 0.45f, 1 });
    for (auto& b : buttons) 
    {
        if (b.selected)
            renderer.drawRect(b.x - 2, b.y - 2, b.w + 4, b.h + 4, { 1.0f, 1.0f, 1.0f, 1 });
        if (b.isFanBtn && isFanOn)
            renderer.drawRect(b.x - 2, b.y - 2, b.w + 4, b.h + 4, { 1.0f, 1.0f, 1.0f, 1 });
        if (b.isStopBtn && isStopped)
            renderer.drawRect(b.x - 2, b.y - 2, b.w + 4, b.h + 4, { 1.0f, 1.0f, 1.0f, 1 });

        renderer.drawTexturedRect(b.x, b.y, b.w, b.h, b.texture);
    }


    // building
    renderer.drawTexturedRect(w * 0.5f, 0, w * 0.5f, h, texHallway);

    // lift shaft
    float shaftX = w * 0.85f;
    float liftWidth = 60;
    renderer.drawTexturedRect(shaftX, 0, liftWidth, h, texLiftShaft);

    // lift cabin
    if (!isOpen)
    {
        // person rendered before closed door
        renderPerson(personDir);
        renderer.drawTexturedRect(shaftX, liftY, liftWidth, floorHeight, texLiftDoorClosed);
    }
    else
    {
        renderer.drawTexturedRect(shaftX, liftY, liftWidth, floorHeight, texLiftDoorOpened);
        // person rendered after opened doord
        renderPerson(personDir);
    }

    // ground
    renderer.drawTexturedRect(0, h - floorHeight, w * 0.5f, floorHeight, texGround);

    // signature
    renderer.drawTexturedRect(0, 0, 400, 50, texSignature);

    if (isDead)
    {
        // dark background
        renderer.drawRect(0, 0, w, h, { 0, 0, 0, 0.5f });

        // you died texture
        renderer.drawTexturedRect(w * 0.5f - 533, h * 0.5f - 150, 1066, 150, texYouDied);

        // reset button
        renderer.drawTexturedRect(resetButton.x, resetButton.y, resetButton.w, resetButton.h, texReset);
    }
}

void App::renderPerson(int personDir)
{
    switch (personDir)
    {
    case 1:
        renderer.drawTexturedRect(personX, personY, personWidth, personHeight, texPersonWalking, true);
        break;
    case 0:
        renderer.drawTexturedRect(personX, personY, personWidth, personHeight, texPersonStanding);
        break;
    case -1:
        renderer.drawTexturedRect(personX, personY, personWidth, personHeight, texPersonWalking);
        break;
    }
}

void App::reset()
{
    for (int i = 0; i < 8; i++)
        buttons[i].selected = false;

    liftY = h - floorHeight * 3; // start on floor 1
    targetFloor = -1;
    liftFloor = 2;

    personX = w * 0.3f; personY = h - floorHeight - personHeight; // ground floor
    personFloor = 1;
    personDir = 0;
    personInside = false;
    isDead = false;

    isOpen = false;
    isMoving = false;
    isStopped = false;
    doorTimer = 0;
    isFanOn = false;

    glfwSetCursor(window, cursorNormal);
}

