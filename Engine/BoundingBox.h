#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H
#include <glm\glm.hpp>

class ENGINE_API BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();
	float xMin, xMax,
		yMin, yMax,
		zMin, zMax;

	void Refresh();
	void Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
	void Transform(glm::mat4 mat);

	void Combine(BoundingBox otherBb);

	void operator=(BoundingBox bb);
private:
	glm::vec3 vertexes[8];

};

#endif
