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

	model->Draw(*shader);
}


