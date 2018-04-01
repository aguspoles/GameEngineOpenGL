#include "stdafx.h"
#include "transform.h"

Transform::Transform()
{
	m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rot = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_modelMatrix = glm::mat4(1.0f);
}

Transform::Transform(const glm::vec3 & pos, const glm::vec3 & rot, const glm::vec3 & scale)
{
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	UpdateModelMatrix();
}

Transform::~Transform()
{
}

glm::mat4 Transform::UpdateModelMatrix()
{
	glm::mat4 posMat = glm::translate(m_pos);
	glm::mat4 scaleMat = glm::scale(m_scale);
	glm::mat4 rotX = glm::rotate(m_rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(m_rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(m_rot.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 rotMat = rotX * rotY * rotZ;

	m_modelMatrix = posMat * rotMat * scaleMat;
	return m_modelMatrix;
}
