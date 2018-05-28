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
	virtual void RenderComposite(glm::mat4 modelMatrix) override final;
	virtual void SetShaderProperties() = 0;

	void SetModel(Model* model);
	void SetShader(Shader* shader);

protected:
	Model* m_model;
	Shader * m_shader;

private:
	void SetShaderInChildren(Composite* comp);

};

