#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H
#include <glm\glm.hpp>
#include "Shader.h"

class ENGINE_API BoundingBox
{
public:
	BoundingBox();
	BoundingBox(bool renderable);
	~BoundingBox();
	float xMin, xMax,
		yMin, yMax,
		zMin, zMax;

	void Combine(BoundingBox otherBb);
	void Refresh();
	void Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
	void Transform(glm::mat4 mat);

	void Render();
	void operator=(BoundingBox bb);
private:
	glm::vec3 vertices[8];

	unsigned int _numIndices;
	static const unsigned int NUM_BUFFERS = 2;
	GLuint _vertexArrayObject;
	GLuint _vertexArrayBuffers[NUM_BUFFERS];

	Shader* _shader;

};

#endif
