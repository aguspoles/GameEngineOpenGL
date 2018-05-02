#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Component.h"
#include "camera.h"

class ENGINE_API Transform : public Component
{
public:
	Transform();
	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	~Transform();

	glm::mat4 UpdateModelMatrix();

	inline glm::mat4 GetModelMatrix() const { return _modelMatrix; }

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

private:

	glm::mat4 _modelMatrix;
};

#endif
