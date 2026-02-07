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

void Camera::processInput(GLFWwindow* window, float deltaTime)
{
	float velocity = speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += glm::vec3(front.x, 0.0f, front.z) * velocity;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= glm::vec3(front.x, 0.0f, front.z) * velocity;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= glm::vec3(right.x, 0.0f, right.z) * velocity;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += glm::vec3(right.x, 0.0f, right.z) * velocity;
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
