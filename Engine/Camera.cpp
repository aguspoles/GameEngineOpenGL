#include "stdafx.h"
#include "Camera.h"
#include "display.h"
#include "Time.h"

Camera* Camera::MainCamera = NULL;

Camera::Camera(glm::vec3 position) :
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = Up;
	Yaw = YAW;
	Pitch = PITCH;
	frustum.setCamInternals(glm::radians(Zoom), (float)Display::SCR_WIDTH / (float)Display::SCR_HEIGHT, 0.1f, 100.0f);
	updateCameraVectors();
}

Camera::~Camera()
{
	if (MainCamera)
		delete MainCamera;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(Zoom), (float)Display::SCR_WIDTH / (float)Display::SCR_HEIGHT, 0.1f, 100.0f);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		Position += Front * velocity * deltaTime;
	}
	if (direction == BACKWARD) {
		Position -= Front * velocity * deltaTime;
	}
	if (direction == LEFT) {
		Position -= Right * velocity * deltaTime;
	}
	if (direction == RIGHT) {
		Position += Right * velocity * deltaTime;
	}

	frustum.setCamDef(Position, Position + Front, Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	yoffset *= MouseSensitivity;
	xoffset *= MouseSensitivity;

	Yaw += xoffset * Time::deltaTime;
	Pitch += yoffset * Time::deltaTime;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 360.0f)
			Pitch = 360.0f;
		if (Pitch < -360.0f)
			Pitch = -360.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;

	frustum.setCamInternals(glm::radians(Zoom), (float)Display::SCR_WIDTH / (float)Display::SCR_HEIGHT, 0.1f, 100.0f);
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));

	frustum.setCamDef(Position, Position + Front, Up);
}
