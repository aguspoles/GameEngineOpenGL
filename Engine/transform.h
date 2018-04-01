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

	inline glm::vec3* GetPos() { return &m_pos; }
	inline glm::vec3* GetRot() { return &m_rot; }
	inline glm::vec3* GetScale() { return &m_scale; }
	inline glm::mat4 GetModelMatrix() const { return m_modelMatrix; }

	inline void SetPos(glm::vec3& pos) { this->m_pos = pos; }
	inline void SetRot(glm::vec3& rot) { this->m_rot = rot; }
	inline void SetScale(glm::vec3& scale) { this->m_scale = scale; }

protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;

	glm::mat4 m_modelMatrix;
};

#endif
