#include "stdafx.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox() : xMin(+10000000), yMin(+10000000), zMin(+10000000),
xMax(-10000000), yMax(-10000000), zMax(-10000000)
{

}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Refresh()
{
	vertexes[0] = glm::vec3(xMax, yMax, zMax);
	vertexes[1] = glm::vec3(xMax, yMax, zMin);
	vertexes[2] = glm::vec3(xMin, yMax, zMax);
	vertexes[3] = glm::vec3(xMin, yMax, zMin);
	vertexes[4] = glm::vec3(xMax, yMin, zMax);
	vertexes[5] = glm::vec3(xMax, yMin, zMin);
	vertexes[6] = glm::vec3(xMin, yMin, zMax);
	vertexes[7] = glm::vec3(xMin, yMin, zMin);
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
		glm::vec4 vertex(vertexes[i], 1.0);
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

void BoundingBox::operator=(BoundingBox bb)
{
	Combine(bb);
	Refresh();
}

