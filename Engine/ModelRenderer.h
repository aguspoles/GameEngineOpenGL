#pragma once
#include "Composite.h"
#include "Model.h"
#include "Shader.h"

class ENGINE_API ModelRenderer :
	public Composite
{
public:
	ModelRenderer();
	virtual ~ModelRenderer();

	virtual void InitComposite() override;
	virtual void UpdateComposite() override;
	virtual void RenderComposite(glm::mat4 modelMatrix) override;
	virtual void SetShaderProperties() = 0;

	Model* model;
	Shader * shader;
	Camera* camera;
protected:

};

