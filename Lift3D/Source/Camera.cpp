#include "../Header/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::update(GLFWwindow* window)
{
	glm::vec3 frontVec;
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(frontVec);

	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		speed = 6.0f;
	else
		speed = 3.0f;

	float velocity = speed * deltaTime;

	glm::vec3 movement(0.0f);

	glm::vec3 forwardXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 rightXZ = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movement += forwardXZ * velocity;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movement -= forwardXZ * velocity;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movement -= rightXZ * velocity;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movement += rightXZ * velocity;



	// floating up and down for TESTING
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;



	return movement;
}


glm::mat4 Camera::getView()
{
	return glm::lookAt(
		position,
		position + front,
		up
	);
}

glm::mat4 Camera::getProjection()
{
	return glm::perspective(
		glm::radians(60.f),
		(float)h / (float)w,
		0.1f,
		100.f
	);
}
