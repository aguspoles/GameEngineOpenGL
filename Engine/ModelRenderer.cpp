#include "stdafx.h"
#include "ModelRenderer.h"


ModelRenderer::ModelRenderer()
{
}


ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::InitComposite()
{
}

void ModelRenderer::UpdateComposite()
{
}

void ModelRenderer::RenderComposite(glm::mat4 modelMatrix)
{
	m_shader->use();

	SetShaderProperties();
}

void ModelRenderer::SetModel(Model * model)
{
	this->m_model = model;
	this->AddComponent(this->m_model->root);
}

void ModelRenderer::SetShader(Shader * shader)
{
	m_shader = shader;
	SetShaderInChildren(this);
}

void ModelRenderer::SetShaderInChildren(Composite * comp)
{
	vector<Component*> components = comp->GetComponents();
	for (size_t i = 0; i < components.size(); i++)
	{
		MeshRenderer* mr = dynamic_cast<MeshRenderer*>(components[i]);
		ModelRenderer* mor = dynamic_cast<ModelRenderer*>(components[i]);
		Composite* composite = dynamic_cast<Composite*>(components[i]);
		if (mr) {
			mr->shader = this->m_shader;
			SetShaderInChildren(mr);
		}
		else if (mor) {
			mor->SetShader(this->m_shader);
		}
		else if(composite) SetShaderInChildren(composite);
	}
}


