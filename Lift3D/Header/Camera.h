#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.2f, 20.0f);
	float playerRadius = 0.4f;
	float yaw = -90.f;
	float pitch = 0.f;
	float speed = 5.0f;
	float sensitivity = 0.1f;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	int w, h;
	
	void update(GLFWwindow* window);
	void processInput(GLFWwindow* window, float deltaTime);

	glm::mat4 getView();
	glm::mat4 getProjection();
};