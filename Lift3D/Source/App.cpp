#include <GL/glew.h>
#include "../Header/App.h"

void App::init()
{
    renderer.init();
}

void App::update()
{
    camera.update(window);
}

void App::render()
{
    glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw outside ground
    renderer.drawCube(
        glm::vec3(0.0f, -groundHeight, 0.0f),
        glm::vec3(buildingWidth * 2.0f, groundHeight, buildingDepth * 2.0f),
        colorGround,
        camera
    );

    // Draw walls and floors
    for (int i = 0; i < 8; i++)
    {
        float y = i * floorHeight;

        // Floor
        renderer.drawCube(
            glm::vec3(0.0f, y, 0.0f),
            glm::vec3(buildingWidth, 0.1f, buildingDepth),  // thin floor
            colorFloor,
            camera
        );

        // Walls (4 walls per floor)
        // Front wall
        renderer.drawCube(
            glm::vec3(0.0f, y + floorHeight / 2, -buildingDepth / 2),
            glm::vec3(buildingWidth, floorHeight, 0.2f),
            colorWall,
            camera
        );
        // Back wall
        renderer.drawCube(
            glm::vec3(0.0f, y + floorHeight / 2, buildingDepth / 2),
            glm::vec3(buildingWidth, floorHeight, 0.2f),
            colorWall,
            camera
        );
        // Left wall
        renderer.drawCube(
            glm::vec3(-buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth),
            colorWall,
            camera
        );
        // Right wall
        renderer.drawCube(
            glm::vec3(buildingWidth / 2, y + floorHeight / 2, 0.0f),
            glm::vec3(0.2f, floorHeight, buildingDepth),
            colorWall,
            camera
        );
    }



}
