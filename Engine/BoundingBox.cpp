#include "stdafx.h"
#include "BoundingBox.h"
#include <vector>
#include "display.h"
#include "Camera.h"


BoundingBox::BoundingBox() : xMin(+10000000), yMin(+10000000), zMin(+10000000),
xMax(-10000000), yMax(-10000000), zMax(-10000000)
{
	for (size_t i = 0; i < 8; i++)
	{
		vertices[i] = { glm::vec4(0,0,0,1.0) };
	}
}

BoundingBox::~BoundingBox()
{
	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void BoundingBox::Refresh()
{
	vertices[0] = glm::vec4(xMax, yMax, zMin, vertices[0].w);
	vertices[1] = glm::vec4(xMin, yMax, zMin, vertices[1].w);
	vertices[2] = glm::vec4(xMax, yMin, zMin, vertices[2].w);
	vertices[3] = glm::vec4(xMin, yMin, zMin, vertices[3].w);
	vertices[4] = glm::vec4(xMax, yMax, zMax, vertices[4].w);
	vertices[5] = glm::vec4(xMin, yMax, zMax, vertices[5].w);
	vertices[6] = glm::vec4(xMax, yMin, zMax, vertices[6].w);
	vertices[7] = glm::vec4(xMin, yMin, zMax, vertices[7].w);
}

BoundingBox BoundingBox::Transform(glm::mat4 mat)
{
	BoundingBox bb;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 transVertex;
		glm::vec4 vertex(vertices[i]);
		transVertex = mat * vertex;

		bb.xMin = std::min(transVertex.x, bb.xMin);
		bb.yMin = std::min(transVertex.y, bb.yMin);
		bb.zMin = std::min(transVertex.z, bb.zMin);

		bb.xMax = std::max(transVertex.x, bb.xMax);
		bb.yMax = std::max(transVertex.y, bb.yMax);
		bb.zMax = std::max(transVertex.z, bb.zMax);

		bb.vertices[i].w = transVertex.w;
	}
	bb.Refresh();
	return bb;
}

glm::vec4 BoundingBox::Getvertex(unsigned int index) const
{
	return vertices[index];
}

void BoundingBox::Combine(BoundingBox otherBb)
{
	xMin = std::min(xMin, otherBb.xMin);
	yMin = std::min(yMin, otherBb.yMin);
	zMin = std::min(zMin, otherBb.zMin);

	xMax = std::max(xMax, otherBb.xMax);
	yMax = std::max(yMax, otherBb.yMax);
	zMax = std::max(zMax, otherBb.zMax);
}

void BoundingBox::InitMesh()
{
	const glm::vec3 verts[8] = {
		glm::vec3(-0.5, -0.5, -0.5),
		glm::vec3(0.5, -0.5, -0.5),
		glm::vec3(0.5,  0.5, -0.5),
		glm::vec3(-0.5,  0.5, -0.5),
		glm::vec3(-0.5, -0.5,  0.5),
		glm::vec3(0.5, -0.5,  0.5),
		glm::vec3(0.5,  0.5,  0.5),
		glm::vec3(-0.5,  0.5,  0.5),
	};
	const std::vector<unsigned int> indices = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};;

	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	//positions buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//indexes buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void BoundingBox::Render(Camera* camera, Shader* shader)
{
	glm::vec3 size = glm::vec3(xMax - xMin, yMax - yMin, zMax - zMin);
	glm::vec3 center = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	shader->use();

	shader->setMat4("model", transform);
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());
	if (this->name == "player")
		shader->setVec3("color", 1.0, 0, 0);
	else shader->setVec3("color", 1.0, 1.0, 1.0);

	glBindVertexArray(_vertexArrayObject);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 0);
	glLineWidth(2);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));

	glBindVertexArray(0);

	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void BoundingBox::Set(const BoundingBox & bb)
{
	xMin = bb.xMin;
	xMax = bb.xMax;
	yMin = bb.yMin;
	yMax = bb.yMax;
	zMin = bb.zMin;
	zMax = bb.zMax;
	vertices[0] = bb.vertices[0];
	vertices[1] = bb.vertices[1];
	vertices[2] = bb.vertices[2];
	vertices[3] = bb.vertices[3];
	vertices[4] = bb.vertices[4];
	vertices[5] = bb.vertices[5];
	vertices[6] = bb.vertices[6];
	vertices[7] = bb.vertices[7];
}



