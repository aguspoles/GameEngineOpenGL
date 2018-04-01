#ifndef LIGHT_H
#define LIGHT_H
#include "Composite.h"
class Shader;

class ENGINE_API Light : public Composite
{
public:
	Light(vector<glm::vec3> vertices, vector<unsigned int> indices);
	~Light();

	virtual void InitComposite()override;
	virtual void UpdateComposite() override;
	virtual void RenderComposite(glm::mat4 modelMatrix) override;

	void SetShader(Shader* shader);
	glm::vec3 Color;

private:
	static const unsigned int NUM_BUFFERS = 2;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];

	Shader* m_shader;
	unsigned int m_numIndices;
};

#endif

