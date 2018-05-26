#ifndef MESHRENDERER_H
#define MESHRENDERER_H
#include "Composite.h"
#include "shader.h"
#include "Mesh.h"
#include <map>

class ENGINE_API MeshRenderer :
	public Composite
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void InitComposite() override final;
	virtual void UpdateComposite() override final;
	virtual void RenderComposite(glm::mat4 modelMatrix) override final;

	void CalculateBB();

	vector<Mesh> meshes;
	Shader* shader;
};

#endif

