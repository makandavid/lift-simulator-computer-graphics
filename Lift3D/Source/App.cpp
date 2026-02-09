#include <GL/glew.h>
#include "../Header/App.h"

void App::init()
{
    renderer.init();

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

    if (elevatorMoving)
    {
        float targetY = targetFloor * floorHeight + floorHeight / 2;

        if (elevatorY < targetY)
        {
            elevatorY += elevatorSpeed * deltaTime;
            //camera.position.y += elevatorSpeed * deltaTime;
        }
        else if (elevatorY > targetY)
        {
            elevatorY -= elevatorSpeed * deltaTime;
            //camera.position.y -= elevatorSpeed * deltaTime;
        }

        if (fabs(elevatorY - targetY) < 0.05f)
        {
            elevatorY = targetY;
            elevatorMoving = false;
            currentFloor = targetFloor;
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
        }
    }
    // TODO prevent elevator from moving

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

        if (f == currentFloor && !elevatorMoving)
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



    testingWithButtons();
}

void App::renderOutsideGround()
{
    // outside ground
    renderer.drawCube(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(groundSize, 0.0f, groundSize),
        colorGround,
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
            camera
        );


        // front wall
        if (i == 0)
        {
            renderer.drawCube(
                glm::vec3(0.0f - buildingWidth / 4 - 0.75f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth / 2 - 1.5f, floorHeight, 0.2f),
                colorWall,
                camera
            );
            renderer.drawCube(
                glm::vec3(0.0f + buildingWidth / 4 + 0.75f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth / 2 - 1.5f, floorHeight, 0.2f),
                colorWall,
                camera
            );
            renderer.drawCube(
                glm::vec3(0.0f, y + floorHeight - 0.75f, buildingDepth / 2),
                glm::vec3(3.0f, floorHeight - 1.5f, 0.2f),
                colorWall,
                camera
            );
        }
        else
        {
            renderer.drawCube(
                glm::vec3(0.0f, y + floorHeight / 2, buildingDepth / 2),
                glm::vec3(buildingWidth, floorHeight, 0.2f),
                colorWall,
                camera
            );
        }
        // back wall
        renderer.drawCube(
            glm::vec3(0.0f, y + floorHeight / 2, -buildingDepth / 2),
            glm::vec3(buildingWidth, floorHeight, 0.2f),
            colorWall,
            camera
        );
        // left wall
        renderer.drawCube(
            glm::vec3(-buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth),
            colorWall,
            camera
        );
        // right wall
        renderer.drawCube(
            glm::vec3(buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth),
            colorWall,
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
        camera
    );

    // left
    renderer.drawCube(
        glm::vec3(shaftX - halfShaft, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftWallThickness, totalHeight, shaftSize),
        colorShaft,
        camera
    );

    // right
    renderer.drawCube(
        glm::vec3(shaftX + halfShaft, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftWallThickness, totalHeight, shaftSize),
        colorShaft,
        camera
    );

    for (int f = -1; f < 7; f++)
    {
        float y = f * floorHeight + floorHeight / 2;

        // draw hole only on elevator floor
        if (f == currentFloor && !elevatorMoving)
            continue;

        renderer.drawCube(
            glm::vec3(shaftX, y, shaftZ + halfShaft),
            glm::vec3(shaftSize, floorHeight, shaftWallThickness),
            colorShaft,
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
        camera
    );
    // back wall
    renderer.drawCube(
        glm::vec3(shaftX, elevatorY, shaftZ - halfShaft),
        glm::vec3(shaftSize, floorHeight, elevatorWallThickness),
        colorElevator,
        camera
    );

    // left wall
    renderer.drawCube(
        glm::vec3(shaftX - halfShaft, elevatorY, shaftZ - 0.1f),
        glm::vec3(elevatorWallThickness, floorHeight, shaftSize + 0.5f),
        colorElevator,
        camera
    );

    // right wall
    renderer.drawCube(
        glm::vec3(shaftX + halfShaft, elevatorY, shaftZ - 0.1f),
        glm::vec3(elevatorWallThickness, floorHeight, shaftSize + 0.5f),
        colorElevator,
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
        camera
    );
}

void App::testingWithButtons()
{
    // TESTING
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        targetFloor = -1;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        targetFloor = 0;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        targetFloor = 1;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        targetFloor = 2;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        targetFloor = 3;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        targetFloor = 4;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        targetFloor = 5;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        targetFloor = 6;
        elevatorMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        doorOpening = true;
        doorClosing = false;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        doorClosing = true;
        doorOpening = false;
    }

}