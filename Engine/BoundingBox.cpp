#include "stdafx.h"
#include "BoundingBox.h"
#include <vector>
#include "display.h"
#include "Camera.h"


BoundingBox::BoundingBox() : xMin(+10000000), yMin(+10000000), zMin(+10000000),
xMax(-10000000), yMax(-10000000), zMax(-10000000), _shader(NULL)
{
	Display::Instance();
	float verts[] = {
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5, -0.5,  0.5,
		0.5, -0.5,  0.5,
		0.5,  0.5,  0.5,
		-0.5,  0.5,  0.5
	};
	std::vector<unsigned int> indices = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	//positions buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts) * 8, verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//indexes buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	_shader = new Shader("../res/basicShader.vs", "../res/basicShader.fs");
}

BoundingBox::BoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	vertices[0] = glm::vec3(xMax, yMax, zMin);
	vertices[1] = glm::vec3(xMin, yMax, zMin);
	vertices[2] = glm::vec3(xMax, yMin, zMin);
	vertices[3] = glm::vec3(xMin, yMin, zMin);
	vertices[4] = glm::vec3(xMax, yMax, zMax);
	vertices[5] = glm::vec3(xMin, yMax, zMax);
	vertices[6] = glm::vec3(xMax, yMin, zMax);
	vertices[7] = glm::vec3(xMin, yMin, zMax);
}

BoundingBox::~BoundingBox()
{
	/*if (_shader) {
		delete _shader;
		_shader = nullptr;
	}*/
	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void BoundingBox::Refresh()
{
	vertices[0] = glm::vec3(xMax, yMax, zMin);
	vertices[1] = glm::vec3(xMin, yMax, zMin);
	vertices[2] = glm::vec3(xMax, yMin, zMin);
	vertices[3] = glm::vec3(xMin, yMin, zMin);
	vertices[4] = glm::vec3(xMax, yMax, zMax);
	vertices[5] = glm::vec3(xMin, yMax, zMax);
	vertices[6] = glm::vec3(xMax, yMin, zMax);
	vertices[7] = glm::vec3(xMin, yMin, zMax);
}

BoundingBox BoundingBox::Transform(glm::mat4 mat)
{
	BoundingBox bb;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 transVertex;
		glm::vec4 vertex(vertices[i], 1.0);
		transVertex = mat * vertex;

		bb.xMin = std::min(transVertex.x, bb.xMin);
		bb.yMin = std::min(transVertex.y, bb.yMin);
		bb.zMin = std::min(transVertex.z, bb.zMin);

		bb.xMax = std::max(transVertex.x, bb.xMax);
		bb.yMax = std::max(transVertex.y, bb.yMax);
		bb.zMax = std::max(transVertex.z, bb.zMax);
	}
	bb.Refresh();
	return bb;
}

glm::vec3 BoundingBox::Getvertex(unsigned int index) const
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

void BoundingBox::Render()
{
	glm::vec3 size = glm::vec3(xMax - xMin, yMax - yMin, zMax - zMin);
	glm::vec3 center = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	glm::mat4 matrix = ModelMatrix * transform;

	_shader->use();

	_shader->setMat4("model", matrix);
	_shader->setMat4("view", Camera::MainCamera->GetViewMatrix());
	_shader->setMat4("projection", Camera::MainCamera->GetProjectionMatrix());

	glBindVertexArray(_vertexArrayObject);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 0);
	glLineWidth(2);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));

	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawElementsBaseVertex(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0, 0);
	////glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
}

//void BoundingBox::operator=(BoundingBox bb)
//{
//	Combine(bb);
//	Refresh();
//}

