#pragma once
#include "../Header/Camera.h"
#include "../Header/Renderer.h"
#include "../Header/Button.h"
#include <GLFW/glfw3.h>
#include <vector>

class App
{
public:
	GLFWwindow* window;
	
	Renderer renderer;
	Camera camera;

	struct Collider
	{
		glm::vec3 min;
		glm::vec3 max;
	};
	std::vector<Collider> colliders;

	float groundSize = 180.0f;     // outside ground (big flat cube)

	float buildingWidth = 20.0f;  // X axis
	float buildingDepth = 20.0f;  // Z axis

	int floorCount = 8;          // total floors including basement
	float floorHeight = 3.0f;

	int currentFloor = 0;
	float elevatorY = 0.0f;
	float elevatorSpeed = 2.0f;

	bool elevatorMoving = false;
	int targetFloor = 0;

	float doorOffset = 0.0f;
	float doorMaxOffset = 0.6f;
	float doorSpeed = 2.0f;

	bool doorOpening = false;
	bool doorClosing = false;
	bool doorOpen = false;

	// colors
	glm::vec3 colorGround = glm::vec3(0.3f, 0.7f, 0.3f);    // green
	glm::vec3 colorFloor = glm::vec3(0.7f, 0.7f, 0.7f);    // light gray
	glm::vec3 colorWall = glm::vec3(0.6f, 0.6f, 0.8f);    // bluish
	glm::vec3 colorShaft = glm::vec3(0.9f, 0.7f, 0.8f);
	glm::vec3 colorElevator = glm::vec3(0.1f, 0.7f, 0.8f);
	glm::vec3 colorElevatorDoor = glm::vec3(0.1f, 0.5f, 0.8f);


	std::vector<ElevatorButton> elevatorButtons;
	std::vector<ElevatorButton> outsideButtons;

	GLFWcursor* cursorNormal = nullptr;
	GLFWcursor* cursorFan = nullptr;

	bool rayIntersectsCube(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 cubeCenter, glm::vec3 cubeSize);
	void processOutsideButtonClick(GLFWwindow* window);



	void init();
	void update(float deltaTime);
	void render();

	void renderOutsideGround();
	void renderBuilding();
	void renderElevator();
	void renderButtons();

	void addCubeCollider(glm::vec3 center, glm::vec3 size);
	void buildColliders();
	void collidesBuilding();
	void collidesElevator();
	bool isColliding(glm::vec3 pos);

	void buildElevatorButtons();
	void buildOutsideButtons();

	void testingWithButtons();
};