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

	inline glm::vec3* GetPos() { return &_pos; }
	inline glm::vec3* GetRot() { return &_rot; }
	inline glm::vec3* GetScale() { return &_scale; }
	inline glm::mat4 GetModelMatrix() const { return _modelMatrix; }

	inline void SetPos(glm::vec3& pos) { this->_pos = pos; }
	inline void SetRot(glm::vec3& rot) { this->_rot = rot; }
	inline void SetScale(glm::vec3& scale) { this->_scale = scale; }


private:
	glm::vec3 _pos;
	glm::vec3 _rot;
	glm::vec3 _scale;

	glm::mat4 _modelMatrix;
};

#endif
