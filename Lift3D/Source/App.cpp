#include <GL/glew.h>
#include "../Header/App.h"
#include "../Header/Util.h"
#include "../Header/Model.h"

void App::init()
{
    renderer.init();

    cursorNormal = loadImageToCursor("Resources/textures/cursor_normal.png");
    cursorFan = loadImageToCursor("Resources/textures/cursor_colored.png");
    glfwSetCursor(window, cursorNormal);

    plantModels[0] = Model("Resources/models/eb_house_plant_01.obj");
    plantModels[1] = Model("Resources/models/potted_plant_obj.obj");
    plantModels[2] = Model("Resources/models/plant.obj");

    lightBulb = Model("Resources/models/halobulb.obj");

    currentFloor = 0;
    elevatorY = currentFloor * floorHeight + floorHeight / 2;

    doorOffset = 0.0f;
    doorOpen = false;
    doorOpening = false;
    doorClosing = false;
}

void App::update(float deltaTime)
{
    buildColliders();

    camera.update(window);

    glm::vec3 movement = camera.processInput(window, deltaTime);

    glm::vec3 pos = camera.position;

    // X movement
    glm::vec3 testPos = pos;
    testPos.x += movement.x;
    if (!isColliding(testPos))
        pos.x = testPos.x;

    // Z movement
    testPos = pos;
    testPos.z += movement.z;
    if (!isColliding(testPos))
        pos.z = testPos.z;

    camera.position = pos;

    for (auto& b : elevatorButtons)
    {
        if (!b.pressed)
            continue;

        switch (b.action)
        {
        case DOOR_OPEN:
            if (!elevatorMoving && !doorOpening && !doorOpen)
            {
                doorClosing = false;
                doorOpening = true;
            }
            break;

        case DOOR_CLOSE:
            if (doorOpen && !doorClosing)
            {
                doorOpen = false;
                doorClosing = true;
            }
            break;

        case STOP:
            elevatorMoving = false;
            doorOpen = false;
            doorOpening = false;
            doorClosing = false;
            isStopped = true;
            floorQueue = std::queue<int>();
            break;

        case FAN:
            glfwSetCursor(window, cursorFan);
            break;

        default:
            break;
        }

        b.pressed = false;
    }


    if (elevatorMoving)
    {
        float targetY = targetFloor * floorHeight + floorHeight / 2;

        if (elevatorY < targetY)
        {
            elevatorY += elevatorSpeed * deltaTime;
            if (isPlayerInsideElevator())
                camera.position.y += elevatorSpeed * deltaTime;
        }
        else if (elevatorY > targetY)
        {
            elevatorY -= elevatorSpeed * deltaTime;
            if (isPlayerInsideElevator())
                camera.position.y -= elevatorSpeed * deltaTime;
        }

        if (fabs(elevatorY - targetY) < 0.05f)
        {
            elevatorY = targetY;
            elevatorMoving = false;
            elevatorFloor = targetFloor;
            if (isPlayerInsideElevator())
                currentFloor = targetFloor;
            doorOpening = true;
            doorClosing = false;
            doorOpen = false;
        }
    }
    if (doorOpening)
    {
        doorOffset += doorSpeed * deltaTime;
        if (doorOffset >= doorMaxOffset)
        {
            doorOffset = doorMaxOffset;
            doorOpening = false;
            doorOpen = true;
            doorTimer = 5.0f;
        }
    }
    if (doorOpen && !doorClosing)
    {
        doorTimer -= deltaTime;
        if (doorTimer <= 0)
        {
            doorTimer = 0;
            doorOpen = false;
            doorClosing = true;
        }
    }
    if (doorClosing)
    {
        doorOffset -= doorSpeed * deltaTime;
        if (doorOffset <= 0.0f)
        {
            doorOffset = 0.0f;
            doorClosing = false;
            doorOpen = false;
            glfwSetCursor(window, cursorNormal);

            while (!floorQueue.empty())
            {
                int nextFloor = floorQueue.front();
                floorQueue.pop();

                if (nextFloor != elevatorFloor)
                {
                    targetFloor = nextFloor;
                    elevatorMoving = true;
                    break;
                }
            }
        }
    }
    if (!floorQueue.empty() && !doorOpening && !doorOpen && !doorClosing && !elevatorMoving && !isStopped)
    {
        targetFloor = floorQueue.front();
        floorQueue.pop();
        elevatorMoving = true;
    }
}

void App::addCubeCollider(glm::vec3 center, glm::vec3 size)
{
    glm::vec3 half = size * 0.5f;

    Collider c;
    c.min = center - half;
    c.max = center + half;

    colliders.push_back(c);
}

void App::buildColliders()
{
    colliders.clear();

    collidesBuilding();
    collidesElevator();
}

void App::collidesBuilding()
{
    for (int i = -1; i < 7; i++)
    {
        float y = i * floorHeight;

        // commented out for testing reasons
        // floor
        /*addCubeCollider(
            glm::vec3(0.0f, y, 0.0f),
            glm::vec3(buildingWidth + 0.5f, 0.1f, buildingDepth + 0.5f)
        );*/

        // front wall
        if (i == 0)
        {
            addCubeCollider(
                glm::vec3(-buildingWidth / 4 - 0.75f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth / 2 - 1.5f, floorHeight, 0.2f)
            );

            addCubeCollider(
                glm::vec3(buildingWidth / 4 + 0.75f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth / 2 - 1.5f, floorHeight, 0.2f)
            );

            addCubeCollider(
                glm::vec3(0.0f, y + floorHeight - 0.75f, buildingDepth / 2),
                glm::vec3(3.0f, floorHeight - 1.5f, 0.2f)
            );
        }
        else
        {
            addCubeCollider(
                glm::vec3(0.0f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth, floorHeight, 0.2f)
            );
        }

        // back wall
        addCubeCollider(
            glm::vec3(0.0f, y + floorHeight / 2, -buildingDepth / 2),
            glm::vec3(buildingWidth, floorHeight, 0.2f)
        );

        // left wall
        addCubeCollider(
            glm::vec3(-buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth)
        );

        // right wall
        addCubeCollider(
            glm::vec3(buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth)
        );
    }
}

void App::collidesElevator()
{
    float shaftSize = 4.0f;
    float shaftX = 0.0f;
    float shaftZ = (shaftSize - buildingDepth) / 2 + 0.2f;

    float shaftWallThickness = 0.2f;
    float elevatorWallThickness = 0.3f;
    float openingWidth = 2.0f;
    float halfShaft = shaftSize / 2.0f;

    float totalHeight = floorCount * floorHeight;

    // SHAFT
    addCubeCollider(
        glm::vec3(shaftX, totalHeight / 2 - floorHeight, shaftZ - halfShaft),
        glm::vec3(shaftSize, totalHeight, shaftWallThickness)
    );
    addCubeCollider(
        glm::vec3(shaftX - halfShaft, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftWallThickness, totalHeight, shaftSize)
    );
    addCubeCollider(
        glm::vec3(shaftX + halfShaft, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftWallThickness, totalHeight, shaftSize)
    );
    for (int f = -1; f < 7; f++)
    {
        float y = f * floorHeight + floorHeight / 2;

        if (f == elevatorFloor && !elevatorMoving)
            continue;

        addCubeCollider(
            glm::vec3(shaftX, y, shaftZ + halfShaft),
            glm::vec3(shaftSize, floorHeight, shaftWallThickness)
        );
    }

    // ELEVATOR
    addCubeCollider(
        glm::vec3(shaftX, elevatorY, shaftZ - halfShaft),
        glm::vec3(shaftSize, floorHeight, elevatorWallThickness)
    );
    addCubeCollider(
        glm::vec3(shaftX - halfShaft, elevatorY, shaftZ),
        glm::vec3(elevatorWallThickness, floorHeight, shaftSize)
    );

    addCubeCollider(
        glm::vec3(shaftX + halfShaft, elevatorY, shaftZ),
        glm::vec3(elevatorWallThickness, floorHeight, shaftSize)
    );

    float sideWidth = (shaftSize - openingWidth) / 2.0f;

    addCubeCollider(
        glm::vec3(
            shaftX - openingWidth / 2 - sideWidth / 2,
            elevatorY,
            shaftZ + halfShaft
        ),
        glm::vec3(sideWidth, floorHeight, elevatorWallThickness)
    );

    addCubeCollider(
        glm::vec3(
            shaftX + openingWidth / 2 + sideWidth / 2,
            elevatorY,
            shaftZ + halfShaft
        ),
        glm::vec3(sideWidth, floorHeight, elevatorWallThickness)
    );

    float t = doorOffset / doorMaxOffset;

    if (t < 1.0f)
    {
        float doorHeight = 3.0f;
        float doorWidth = 1.0f;
        float doorDepth = 0.3f;
        float doorZ = shaftZ + shaftSize / 2;

        float currentWidth = doorWidth * (1.0f - t);

        float leftEdge = -shaftSize / 2 + 1.0f;
        float rightEdge = shaftSize / 2 - 1.0f;

        addCubeCollider(
            glm::vec3(leftEdge + currentWidth / 2, elevatorY, doorZ),
            glm::vec3(currentWidth, doorHeight, doorDepth)
        );

        addCubeCollider(
            glm::vec3(rightEdge - currentWidth / 2, elevatorY, doorZ),
            glm::vec3(currentWidth, doorHeight, doorDepth)
        );
    }

}

bool App::isColliding(glm::vec3 pos)
{
    float radius = 0.4f; // player size

    glm::vec3 min = pos - glm::vec3(radius);
    glm::vec3 max = pos + glm::vec3(radius);

    for (auto& c : colliders)
    {
        bool overlap =
            min.x <= c.max.x && max.x >= c.min.x &&
            min.y <= c.max.y && max.y >= c.min.y &&
            min.z <= c.max.z && max.z >= c.min.z;

        if (overlap)
            return true;
    }

    return false;
}


void App::render()
{
    glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderOutsideGround();
    renderBuilding();
    renderElevator();
    renderButtons();
    renderPlants();
    renderBulbs();
}

void App::renderOutsideGround()
{
    // outside ground
    renderer.drawCube(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(groundSize, 0.0f, groundSize),
        colorGround,
        renderer.texGrass,
        camera
    );

}

void App::renderBuilding()
{
    float shaftSize = 4.0f;
    float halfShaft = shaftSize / 2.0f;

    float floorThickness = 0.1f;
    float floorWidth = buildingWidth + 0.5f;
    float floorDepth = buildingDepth + 0.5f;

    // walls and floors
    for (int i = -1; i < 7; i++)
    {
        float y = i * floorHeight;

        // floor
        renderer.drawCube(
            glm::vec3(
                -floorWidth / 4 - halfShaft / 2,
                y,
                0.0f
            ),
            glm::vec3(
                floorWidth / 2 - halfShaft,
                floorThickness,
                floorDepth
            ),
            colorFloor,
            renderer.texFloor,
            camera
        );
        renderer.drawCube(
            glm::vec3(
                floorWidth / 4 + halfShaft / 2,
                y,
                0.0f
            ),
            glm::vec3(
                floorWidth / 2 - halfShaft,
                floorThickness,
                floorDepth
            ),
            colorFloor,
            renderer.texFloor,
            camera
        );

        renderer.drawCube(
            glm::vec3(
                0.0f,
                y,
                halfShaft
            ),
            glm::vec3(
                shaftSize,
                floorThickness,
                floorDepth - shaftSize - 0.6f
            ),
            colorFloor,
            renderer.texFloor,
            camera
        );


        // front wall
        if (i == 0)
        {
            renderer.drawCube(
                glm::vec3(0.0f - buildingWidth / 4 - 0.75f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth / 2 - 1.5f, floorHeight, 0.2f),
                colorWall,
                renderer.texWall,
                camera
            );
            renderer.drawCube(
                glm::vec3(0.0f + buildingWidth / 4 + 0.75f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth / 2 - 1.5f, floorHeight, 0.2f),
                colorWall,
                renderer.texWall,
                camera
            );
            renderer.drawCube(
                glm::vec3(0.0f, y + floorHeight - 0.75f, buildingDepth / 2),
                glm::vec3(3.0f, floorHeight - 1.5f, 0.2f),
                colorWall,
                renderer.texWall,
                camera
            );
        }
        else
        {
            renderer.drawCube(
                glm::vec3(0.0f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth, floorHeight, 0.2f),
                colorWall,
                renderer.texWall,
                camera
            );
        }
        // back wall
        renderer.drawCube(
            glm::vec3(0.0f, y + floorHeight / 2, -buildingDepth / 2),
            glm::vec3(buildingWidth, floorHeight, 0.2f),
            colorWall,
            renderer.texWall,
            camera
        );
        // left wall
        renderer.drawCube(
            glm::vec3(-buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth),
            colorWall,
            renderer.texWall,
            camera
        );
        // right wall
        renderer.drawCube(
            glm::vec3(buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth),
            colorWall,
            renderer.texWall,
            camera
        );
    }
}

void App::renderElevator()
{

    float shaftSize = 4.0f;
    float shaftX = 0.0f;
    float shaftZ = (shaftSize - buildingDepth) / 2 + 0.2f;
    float totalHeight = floorCount * floorHeight;

    float shaftWallThickness = 0.2f;
    float elevatorWallThickness = 0.3f;
    float openingWidth = 2.0f;
    float halfShaft = shaftSize / 2.0f;

    // SHAFT
    // back
    renderer.drawCube(
        glm::vec3(shaftX, totalHeight / 2 - floorHeight, shaftZ - halfShaft),
        glm::vec3(shaftSize, totalHeight, shaftWallThickness),
        colorShaft,
        renderer.texShaft,
        camera
    );

    // left
    renderer.drawCube(
        glm::vec3(shaftX - halfShaft, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftWallThickness, totalHeight, shaftSize),
        colorShaft,
        renderer.texShaft,
        camera
    );

    // right
    renderer.drawCube(
        glm::vec3(shaftX + halfShaft, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftWallThickness, totalHeight, shaftSize),
        colorShaft,
        renderer.texShaft,
        camera
    );

    for (int f = -1; f < 7; f++)
    {
        float y = f * floorHeight + floorHeight / 2;

        // draw hole only on elevator floor
        if (f == elevatorFloor && !elevatorMoving)
            continue;

        renderer.drawCube(
            glm::vec3(shaftX, y, shaftZ + halfShaft),
            glm::vec3(shaftSize, floorHeight, shaftWallThickness),
            colorShaft,
            renderer.texShaft,
            camera
        );
    }

    // ELEVATOR
    // floor
    renderer.drawCube(
        glm::vec3(
            0.0f,
            elevatorY - floorHeight / 2,
            shaftZ
        ),
        glm::vec3(
            shaftSize,
            0.1f,
            shaftSize
        ),
        colorFloor,
        renderer.texFloor,
        camera
    );
    // ceiling
    renderer.drawCube(
        glm::vec3(
            0.0f,
            elevatorY + floorHeight / 2,
            shaftZ
        ),
        glm::vec3(
            shaftSize,
            0.1f,
            shaftSize
        ),
        colorFloor,
        renderer.texFloor,
        camera
    );
    // back wall
    renderer.drawCube(
        glm::vec3(shaftX, elevatorY, shaftZ - halfShaft),
        glm::vec3(shaftSize, floorHeight, elevatorWallThickness),
        colorElevator,
        renderer.texElevator,
        camera
    );

    // left wall
    renderer.drawCube(
        glm::vec3(shaftX - halfShaft, elevatorY, shaftZ - 0.1f),
        glm::vec3(elevatorWallThickness, floorHeight, shaftSize + 0.5f),
        colorElevator,
        renderer.texElevator,
        camera
    );

    // right wall
    renderer.drawCube(
        glm::vec3(shaftX + halfShaft, elevatorY, shaftZ - 0.1f),
        glm::vec3(elevatorWallThickness, floorHeight, shaftSize + 0.5f),
        colorElevator,
        renderer.texElevator,
        camera
    );

    // front wall split (door opening)
    float sideWidth = (shaftSize - openingWidth) / 2.0f;

    renderer.drawCube(
        glm::vec3(
            shaftX - openingWidth / 2 - sideWidth / 2,
            elevatorY,
            shaftZ + halfShaft
        ),
        glm::vec3(sideWidth, floorHeight, elevatorWallThickness),
        colorElevator,
        renderer.texElevator,
        camera
    );

    renderer.drawCube(
        glm::vec3(
            shaftX + openingWidth / 2 + sideWidth / 2,
            elevatorY,
            shaftZ + halfShaft
        ),
        glm::vec3(sideWidth, floorHeight, elevatorWallThickness),
        colorElevator,
        renderer.texElevator,
        camera
    );


    float doorHeight = 3.0f;
    float doorWidth = 1.0f;
    float doorDepth = 0.3f;
    float doorZ = shaftZ + shaftSize / 2;

    float t = doorOffset / doorMaxOffset;
    float currentWidth = doorWidth * (1.0f - t);

    // door edges
    float leftEdge = -shaftSize / 2 + 1.0f;
    float rightEdge = shaftSize / 2 - 1.0f;

    // left door
    renderer.drawCube(
        glm::vec3(
            leftEdge + currentWidth / 2,
            elevatorY,
            doorZ
        ),
        glm::vec3(currentWidth, doorHeight, doorDepth),
        colorElevatorDoor,
        renderer.texDoor,
        camera
    );
    // right door
    renderer.drawCube(
        glm::vec3(
            rightEdge - currentWidth / 2,
            elevatorY,
            doorZ
        ),
        glm::vec3(currentWidth, doorHeight, doorDepth),
        colorElevatorDoor,
        renderer.texDoor,
        camera
    );
}

void App::buildElevatorButtons()
{
    elevatorButtons.clear();

    float panelX = 1.8f;
    float startY = elevatorY;
    float z1 = -6.5f;
    float z2 = z1 + 0.1;

    float dy = 0.1f;

    glm::vec3 size(0.01f, 0.1f, 0.1f);

    elevatorButtons.push_back({ {panelX,startY,z1}, size,  FLOOR_SU });
    elevatorButtons.push_back({ {panelX,startY,z2}, size, FLOOR_PR });
    elevatorButtons.push_back({ {panelX,startY - dy,z1}, size, FLOOR_1 });
    elevatorButtons.push_back({ {panelX,startY - dy,z2}, size, FLOOR_2 });
    elevatorButtons.push_back({ {panelX,startY - 2 * dy,z1}, size, FLOOR_3 });
    elevatorButtons.push_back({ {panelX,startY - 2 * dy,z2}, size, FLOOR_4 });
    elevatorButtons.push_back({ {panelX,startY - 3 * dy,z1}, size, FLOOR_5 });
    elevatorButtons.push_back({ {panelX,startY - 3 * dy,z2}, size, FLOOR_6 });

    elevatorButtons.push_back({ {panelX,startY - 4 * dy,z1}, size, DOOR_OPEN });
    elevatorButtons.push_back({ {panelX,startY - 4 * dy,z2}, size, DOOR_CLOSE });
    elevatorButtons.push_back({ {panelX,startY - 5 * dy,z1}, size, STOP });
    elevatorButtons.push_back({ {panelX,startY - 5 * dy,z2}, size, FAN });
}

void App::buildOutsideButtons()
{
    outsideButtons.clear();

    for (int i = -1; i < 7; i++)
    {
        float y = i * floorHeight + 1.0f;

        outsideButtons.push_back({
            glm::vec3(1.2f, y, -5.6f),
            glm::vec3(0.1f, 0.1f, 0.01f),
            i
            });
    }
}

void App::renderButtons()
{
    buildElevatorButtons();
    buildOutsideButtons();

    for (auto& b : elevatorButtons)
    {
        GLuint tex = 0;

        switch (b.action)
        {
            case ButtonAction::FLOOR_1:    tex = renderer.texBtn1; break;
            case ButtonAction::FLOOR_2:    tex = renderer.texBtn2; break;
            case ButtonAction::FLOOR_3:    tex = renderer.texBtn3; break;
            case ButtonAction::FLOOR_4:    tex = renderer.texBtn4; break;
            case ButtonAction::FLOOR_5:    tex = renderer.texBtn5; break;
            case ButtonAction::FLOOR_6:    tex = renderer.texBtn6; break;
            case ButtonAction::FLOOR_SU:  tex = renderer.texBtnSU; break;
            case ButtonAction::FLOOR_PR:    tex = renderer.texBtnPR; break;
            case ButtonAction::DOOR_OPEN:      tex = renderer.texBtnOpen; break;
            case ButtonAction::DOOR_CLOSE:     tex = renderer.texBtnClose; break;
            case ButtonAction::STOP:      tex = renderer.texBtnStop; break;
            case ButtonAction::FAN:       tex = renderer.texBtnFan; break;
            case ButtonAction::CALL_ELEVATOR: tex = renderer.texBtnOpen; break;
        }

        renderer.drawCube(
            b.position,
            b.size,
            glm::vec3(1, 0, 0),
            tex,
            camera
        );
    }

    for (auto& b : outsideButtons)
    {
        renderer.drawCube(
            b.position,
            b.size,
            glm::vec3(1, 0, 0),
            renderer.texBtnOpen,
            camera
        );
    }

}

void App::renderPlants()
{
    float shaftSize = 4.0f;
    float shaftZ = (shaftSize - buildingDepth) / 2 + 0.2f;
    float halfShaft = shaftSize / 2.0f;

    float leftX = -shaftSize / 2 + 0.6f;
    float rightX = shaftSize / 2 - 0.6f;

    float plantZ = shaftZ + halfShaft + 1.5f;

    int modelIndex = 0;
    float scale = 1;

    int plantIndex = 0;

    for (int f = -1; f < floorCount - 1; f++)
    {
        float y = f * floorHeight + 0.1f;

        float scale = (plantIndex % 3 == 2) ? 0.1f : 0.01f;

        Model model = plantModels[plantIndex % 3];

        plants[plantIndex++] = { model, { leftX,  y, plantZ }, scale };
        plants[plantIndex++] = { model, { rightX, y, plantZ }, scale };
    }

    for (auto& p : plants)
    {
        renderer.drawModel(p.model, p.position, p.scale, camera, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void App::renderBulbs()
{
    float shaftSize = 4.0f;
    float shaftZ = (shaftSize - buildingDepth) / 2 + 0.2f;

    std::vector<glm::vec3> lightPositions;

    for (int i = -1; i < floorCount - 1; i++) {
        float y = i * floorHeight + floorHeight - 0.1f;
        lightPositions.push_back(glm::vec3(0.0f, y, 0.0f));
    }

    for (auto& pos : lightPositions) {
        renderer.drawModel(lightBulb, pos, 0.05f, camera, glm::vec3(180.0f, 0.0f, 0.0f));
    }


    // Elevator ceiling light moves with elevator
    glm::vec3 elevatorLightPos(0.0f, elevatorY + 0.9f, shaftZ); // Y offset inside elevator
    renderer.drawModel(lightBulb, elevatorLightPos, 0.05f, camera, glm::vec3(180.0f, 0.0f, 0.0f));


}



bool rayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 boxMin, glm::vec3 boxMax, float& t)
{
    float tMin = (boxMin.x - rayOrigin.x) / rayDir.x;
    float tMax = (boxMax.x - rayOrigin.x) / rayDir.x;
    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (boxMin.y - rayOrigin.y) / rayDir.y;
    float tyMax = (boxMax.y - rayOrigin.y) / rayDir.y;
    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (boxMin.z - rayOrigin.z) / rayDir.z;
    float tzMax = (boxMax.z - rayOrigin.z) / rayDir.z;
    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax)) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    t = tMin;
    return true;
}

void App::processOutsideButtonClick(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glm::vec3 rayOrigin = camera.position;
        glm::vec3 rayDir = glm::normalize(camera.front); // camera forward

        float t;

        // Check every outside button
        for (auto& b : outsideButtons)
        {
            glm::vec3 min = b.position - b.size * 0.5f;
            glm::vec3 max = b.position + b.size * 0.5f;

            if (rayIntersectsAABB(rayOrigin, rayDir, min, max, t))
            {
                if (t > 2.0f) return;
                b.pressed = true;
                if (b.action <= 6) {
                    floorQueue.push(b.action);
                    isStopped = false;
                }
                return;
            }
        }
        for (auto& b : elevatorButtons)
        {
            glm::vec3 min = b.position - b.size * 0.5f;
            glm::vec3 max = b.position + b.size * 0.5f;

            if (rayIntersectsAABB(rayOrigin, rayDir, min, max, t))
            {
                if (t > 2.0f) return;
                b.pressed = true;
                if (b.action <= 6) {
                    floorQueue.push(b.action);
                    isStopped = false;
                }
                return;
            }
        }
    }
}

bool App::isPlayerInsideElevator()
{
    float shaftSize = 4.0f;
    float shaftZ = (shaftSize - buildingDepth) / 2 + 0.2f;
    float halfShaft = shaftSize / 2.0f;

    // elevator box limits
    float minX = -halfShaft;
    float maxX = halfShaft;

    float minZ = shaftZ - halfShaft;
    float maxZ = shaftZ + halfShaft;

    float minY = elevatorY - floorHeight / 2.0f;
    float maxY = elevatorY + floorHeight / 2.0f;

    const glm::vec3& p = camera.position;

    return
        p.x >= minX && p.x <= maxX &&
        p.y >= minY && p.y <= maxY &&
        p.z >= minZ && p.z <= maxZ;
}


void App::testingWithButtons()
{
    //// TESTING
    //if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    //{
    //    targetFloor = -1;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    //{
    //    targetFloor = 0;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    //{
    //    targetFloor = 1;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    //{
    //    targetFloor = 2;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    //{
    //    targetFloor = 3;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    //{
    //    targetFloor = 4;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    //{
    //    targetFloor = 5;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    //{
    //    targetFloor = 6;
    //    elevatorMoving = true;
    //}
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    //{
    //    doorOpening = true;
    //    doorClosing = false;
    //}
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    //{
    //    doorClosing = true;
    //    doorOpening = false;
    //}

}