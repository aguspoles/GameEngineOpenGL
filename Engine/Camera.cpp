#include "stdafx.h"
#include "Camera.h"
#include "display.h"
#include "Time.h"

Camera* Camera::MainCamera = NULL;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
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

std::vector<Plane> Camera::FrustumPlanes()
{
	std::vector<Plane> res;
	glm::mat4 viewProj =  GetProjectionMatrix() * GetViewMatrix();
	Plane rightPlane(
		viewProj[3][0] - viewProj[0][0],
		viewProj[3][1] - viewProj[0][1],
		viewProj[3][2] - viewProj[0][2],
		viewProj[3][3] - viewProj[0][3]
	);
	res.push_back(rightPlane);
	Plane leftPlane(
		viewProj[3][0] + viewProj[0][0],
		viewProj[3][1] + viewProj[0][1],
		viewProj[3][2] + viewProj[0][2],
		viewProj[3][3] + viewProj[0][3]
	);
	res.push_back(leftPlane);
	Plane nearPlane(
		0,
		0,
		viewProj[2][2],
		0
	);
	res.push_back(nearPlane);
	Plane farPlane(
		viewProj[3][0] - viewProj[2][0],
		viewProj[3][1] - viewProj[2][1],
		viewProj[3][2] - viewProj[2][2],
		viewProj[3][3] - viewProj[2][3]
	);
	res.push_back(farPlane);
	Plane topPlane(
		viewProj[3][0] - viewProj[1][0],
		viewProj[3][1] - viewProj[1][1],
		viewProj[3][2] - viewProj[1][2],
		viewProj[3][3] - viewProj[1][3]
	);
	res.push_back(topPlane);
	Plane bottomPlane(
		viewProj[3][0] + viewProj[1][0],
		viewProj[3][1] + viewProj[1][1],
		viewProj[3][2] + viewProj[1][2],
		viewProj[3][3] + viewProj[1][3]
	);
	res.push_back(bottomPlane);
	return res;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)			
		Position += Front * velocity * deltaTime;
	if (direction == BACKWARD)		   
		Position -= Front * velocity * deltaTime;
	if (direction == LEFT)			  
		Position -= Right * velocity * deltaTime;
	if (direction == RIGHT)			  
		Position += Right * velocity * deltaTime;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	yoffset *= MouseSensitivity;
	xoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
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
}
