#pragma once
#include "../Header/Camera.h"
#include "../Header/Renderer.h"
#include "../Header/Button.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <queue>
#include "Model.h"

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
	int elevatorFloor = 0;
	float elevatorY = 0.0f;
	float elevatorSpeed = 2.0f;

	bool elevatorMoving = false;
	int targetFloor = 0;

	float doorOffset = 0.0f;
	float doorMaxOffset = 0.6f;
	float doorSpeed = 2.0f;
	float doorTimer = 0.0f;

	bool doorOpening = false;
	bool doorClosing = false;
	bool doorOpen = false;
	bool isStopped = false;
	bool isFanOn = false;

	int nextTargetFloor = -1; // -1 means no pending floor to go to immediately after doors close
	std::queue<int> floorQueue;


	// colors
	glm::vec3 colorGround = glm::vec3(0.3f, 0.7f, 0.3f);    // green
	glm::vec3 colorFloor = glm::vec3(0.7f, 0.7f, 0.7f);    // light gray
	glm::vec3 colorWall = glm::vec3(0.6f, 0.6f, 0.8f);    // bluish
	glm::vec3 colorShaft = glm::vec3(0.9f, 0.7f, 0.8f);
	glm::vec3 colorElevator = glm::vec3(0.1f, 0.7f, 0.8f);
	glm::vec3 colorElevatorDoor = glm::vec3(0.1f, 0.5f, 0.8f);

	Model plantModels[3];
	struct PlantInstance
	{
		Model model;
		glm::vec3 position;
		float scale;
	};
	PlantInstance plants[2 * 8];

	Model lightBulb;


	std::vector<ElevatorButton> elevatorButtons;
	std::vector<ElevatorButton> outsideButtons;

	GLFWcursor* cursorNormal = nullptr;
	GLFWcursor* cursorFan = nullptr;

	bool rayIntersectsCube(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 cubeCenter, glm::vec3 cubeSize);
	void processButtonClick(GLFWwindow* window);



	void init();
	void update(float deltaTime);
	void render();

	void renderOutsideGround();
	void renderBuilding();
	void renderElevator();
	void renderButtons();
	void renderPlants();
	void renderBulbs();

	void collectBulbLights();
	void collectButtonLights();

	void addCubeCollider(glm::vec3 center, glm::vec3 size);
	void buildColliders();
	void collidesBuilding();
	void collidesElevator();
	bool isColliding(glm::vec3 pos);
	bool isPlayerInsideElevator();
	int actionToFloor(ButtonAction a);


	void buildElevatorButtons();
	void buildOutsideButtons();
	void updateElevatorButtonPositions();

	void testingWithButtons();
};