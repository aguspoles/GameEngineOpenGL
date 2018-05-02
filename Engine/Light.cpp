#include "stdafx.h"
#include "Light.h"
#include "Camera.h"
#include "Time.h"
#include "shader.h"
#include <iostream>

Light::Light(vector<glm::vec3> vertices, vector<unsigned int> indices)
{
	m_numIndices = indices.size();
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	//positions buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//indexes buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


Light::~Light()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Light::InitComposite()
{
	Color = glm::vec3(1.0, 1.0, 0.5);
	transform->position.x = 1.2;
	transform->position.y = 1.0;
	transform->position.z = -2.0;
	transform->scale.x = 0.3;
	transform->scale.y = 0.3;
	transform->scale.z = 0.3;
	transform->UpdateModelMatrix();
}

void Light::UpdateComposite()
{
	transform->position.z = cos(Time::GetTime()) * 5;
	transform->UpdateModelMatrix();
}

void Light::RenderComposite(glm::mat4 modelMatrix)
{
	m_shader->use();

	m_shader->setMat4("model", GetModelMatrix());
	m_shader->setMat4("view", Camera::MainCamera->GetViewMatrix());
	m_shader->setMat4("projection", Camera::MainCamera->GetProjectionMatrix());

	glBindVertexArray(m_vertexArrayObject);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}

void Light::SetShader(Shader * shader)
{
	m_shader = shader;
}


