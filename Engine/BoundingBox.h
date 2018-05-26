#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H
#include <glm\glm.hpp>
#include <assimp/Importer.hpp>
#include <string>
#include "Shader.h"
class Camera;

class ENGINE_API BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	float xMin, xMax,
		yMin, yMax,
		zMin, zMax;
	glm::vec4 vertices[8];

	std::string name;
	Shader* shader;

	void Combine(BoundingBox otherBb);
	void Refresh();
	BoundingBox Transform(glm::mat4 mat);
	glm::vec4 Getvertex(unsigned int index) const;
	//glm::vec3 getVertexP(glm::vec3 normal);

	void InitMesh();
	void Render(Camera* camera);

	void Set(const BoundingBox& bb);

private:

	static const unsigned int NUM_BUFFERS = 2;
	GLuint _vertexArrayObject;
	GLuint _vertexArrayBuffers[NUM_BUFFERS];
};

#endif
