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
    camera.update(window);

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

void App::render()
{
    glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // outside ground
    renderer.drawCube(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(groundSize, 0.0f, groundSize),
        colorGround,
        camera
    );

    renderBuilding();

    float shaftSize = 4.0f;
    float shaftX = 0.0f;
    float shaftZ = (shaftSize - buildingDepth) / 2 + 0.2f;
    float totalHeight = floorCount * floorHeight;

    // shaft
    renderer.drawCube(
        glm::vec3(shaftX, totalHeight / 2 - floorHeight, shaftZ),
        glm::vec3(shaftSize, totalHeight, shaftSize),
        colorShaft,
        camera
    );
    // elevator
    renderer.drawCube(
        glm::vec3(shaftX, elevatorY, shaftZ),
        glm::vec3(shaftSize + 0.1f, floorHeight, shaftSize + 0.1f),
        colorElevator,
        camera
    );
    renderer.drawCube(
        glm::vec3(shaftX, elevatorY, shaftZ),
        glm::vec3(shaftSize - 0.1f, floorHeight, shaftSize - 0.1f),
        colorElevator,
        camera
    );

    float doorHeight = 1.5f;
    float doorWidth = 1.0f;
    float doorDepth = 0.2f;
    float doorZ = shaftZ + shaftSize / 2;

    float t = doorOffset / doorMaxOffset;
    float currentWidth = doorWidth * (1.0f - t);

    // elevator edges
    float leftEdge = -shaftSize / 2 + 1.0f;
    float rightEdge = shaftSize / 2 - 1.0f;

    // left door
    renderer.drawCube(
        glm::vec3(
            leftEdge + currentWidth / 2,
            elevatorY - doorHeight / 2,
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
            elevatorY - doorHeight / 2,
            doorZ
        ),
        glm::vec3(currentWidth, doorHeight, doorDepth),
        colorElevatorDoor,
        camera
    );











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

void App::renderBuilding()
{
    // walls and floors
    for (int i = -1; i < 7; i++)
    {
        float y = i * floorHeight;

        // floor
        renderer.drawCube(
            glm::vec3(0.0f, y, 0.0f),
            glm::vec3(buildingWidth + 0.5f, 0.1f, buildingDepth + 0.5f),
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