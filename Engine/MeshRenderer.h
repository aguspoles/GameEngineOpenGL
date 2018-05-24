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

	virtual void InitComposite() override;
	virtual void UpdateComposite() override;
	virtual void RenderComposite(glm::mat4 modelMatrix) override;
	virtual void SetShaderProperties();

	void SetShader(Shader* shader);
	void SetMesh(Mesh* mesh);

	Shader* GetShader();
	Camera* camera;

	vector<Mesh> meshes;
	BoundingBox BB;
protected:
	Shader* m_shader;

};

#endif

