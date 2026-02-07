#pragma once
#include "../Header/Camera.h"
#include "../Header/Renderer.h"
#include <GLFW/glfw3.h>

class App
{
public:
	GLFWwindow* window;
	
	Renderer renderer;
	Camera camera;

	// Building dimensions
	int floorCount = 8;          // total floors including basement
	float floorHeight = 3.0f;

	float buildingWidth = 20.0f;  // X axis
	float buildingDepth = 20.0f;  // Z axis

	float wallThickness = 0.5f;   // walls
	float floorThickness = 0.2f;  // each floor height thickness
	float groundHeight = 0.1f;    // outside ground thickness
	float groundThickness = 0.1f;
	float groundSize = 80.0f;     // outside ground (big flat cube)

	glm::vec3 colorGround = glm::vec3(0.3f, 0.7f, 0.3f);    // green
	glm::vec3 colorFloor = glm::vec3(0.7f, 0.7f, 0.7f);    // light gray
	glm::vec3 colorWall = glm::vec3(0.6f, 0.6f, 0.8f);    // bluish



	void init();
	void update();
	void render();
};