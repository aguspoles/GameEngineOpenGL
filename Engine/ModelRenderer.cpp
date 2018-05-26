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
	shader->use();

	SetShaderProperties();
}

void ModelRenderer::SetModel(Model * model)
{
	this->model = model;
	this->AddComponent(this->model->root);
	SetPropertiesInChildren(this);
}

void ModelRenderer::SetPropertiesInChildren(Composite * comp)
{
	vector<MeshRenderer*> components = comp->GetComponents<MeshRenderer>();
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->shader = this->shader;
		components[i]->camera = this->camera;
		SetPropertiesInChildren(components[i]);
	}
}


