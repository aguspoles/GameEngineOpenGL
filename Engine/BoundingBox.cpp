#include "stdafx.h"
#include "BoundingBox.h"
#include <vector>

BoundingBox::BoundingBox() : xMin(+10000000), yMin(+10000000), zMin(+10000000),
xMax(-10000000), yMax(-10000000), zMax(-10000000), _shader(NULL)
{
	
}

BoundingBox::BoundingBox(bool renderable) : xMin(+10000000), yMin(+10000000), zMin(+10000000),
xMax(-10000000), yMax(-10000000), zMax(-10000000), _shader(NULL)
{
	//if (renderable) {
	//	std::vector<unsigned int> indices = { 0, 1, 2,
	//		0, 2, 3,

	//		6, 5, 4,
	//		7, 6, 4,

	//		10, 9, 8,
	//		11, 10, 8,

	//		12, 13, 14,
	//		12, 14, 15,

	//		16, 17, 18,
	//		16, 18, 19,

	//		22, 21, 20,
	//		23, 22, 20
	//	};
	//	_numIndices = indices.size();
	//	glGenVertexArrays(1, &_vertexArrayObject);
	//	glBindVertexArray(_vertexArrayObject);
	//	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	//	//positions buffer
	//	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * 8, &vertices[0], GL_STATIC_DRAW);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);

	//	//indexes buffer
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	//	glBindVertexArray(0);

	//	_shader = new Shader("../res/lightSource.vs", "../res/lightSource.fs");
	//}
}

BoundingBox::~BoundingBox()
{
	if (_shader) delete _shader;
	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void BoundingBox::Refresh()
{
	vertices[0] = glm::vec3(xMax, yMax, zMax);
	vertices[1] = glm::vec3(xMax, yMax, zMin);
	vertices[2] = glm::vec3(xMin, yMax, zMax);
	vertices[3] = glm::vec3(xMin, yMax, zMin);
	vertices[4] = glm::vec3(xMax, yMin, zMax);
	vertices[5] = glm::vec3(xMax, yMin, zMin);
	vertices[6] = glm::vec3(xMin, yMin, zMax);
	vertices[7] = glm::vec3(xMin, yMin, zMin);
}

void BoundingBox::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
	glm::mat4 matTrans = glm::translate(pos);
	glm::mat4 matSca = glm::scale(sca);
	glm::mat4 matRotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 matRotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 matRotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));

	glm::mat4 mat = matTrans * matRotX * matRotY * matRotZ * matSca;

	Transform(mat);
}

void BoundingBox::Transform(glm::mat4 mat)
{
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 transVertex;
		glm::vec4 vertex(vertices[i], 1.0);
		transVertex = mat * vertex;

		xMin = std::min(transVertex.x, xMin);
		yMin = std::min(transVertex.y, yMin);
		zMin = std::min(transVertex.z, zMin);

		xMax = std::max(transVertex.x, xMax);
		yMax = std::max(transVertex.y, yMax);
		zMax = std::max(transVertex.z, zMax);
	}
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
	/*_shader->use();
	glBindVertexArray(_vertexArrayObject);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElementsBaseVertex(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);*/
}

void BoundingBox::operator=(BoundingBox bb)
{
	Combine(bb);
	Refresh();
}

