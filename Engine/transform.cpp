#include "stdafx.h"
#include "transform.h"
#include "MeshRenderer.h"
#include "BoundingBox.h"

Transform::Transform()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_modelMatrix = glm::mat4(1.0f);
}

Transform::Transform(const glm::vec3 & pos, const glm::vec3 & rot, const glm::vec3 & scal)
{
	position = pos;
	rotation = rot;
	scale = scal;
	UpdateModelMatrix();
}

Transform::~Transform()
{
}

glm::mat4 Transform::UpdateModelMatrix()
{
	glm::mat4 posMat = glm::translate(position);
	glm::mat4 scaleMat = glm::scale(scale);
	glm::mat4 rotX = glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 rotMat = rotX * rotY * rotZ;

	_modelMatrix = posMat * scaleMat * rotMat;

	return _modelMatrix;
}
