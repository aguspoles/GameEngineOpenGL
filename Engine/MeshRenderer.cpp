#include "stdafx.h"
#include "MeshRenderer.h"
#include "Camera.h"


MeshRenderer::MeshRenderer() : m_shader(NULL)
{
}


MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::InitComposite()
{
}

void MeshRenderer::UpdateComposite()
{
}

void MeshRenderer::RenderComposite(glm::mat4 modelMatrix)
{
	m_shader->use();

	SetShaderProperties();

	m_model->Draw(*m_shader);
}

void MeshRenderer::SetShader(Shader* shader)
{
	m_shader = shader;
}

void MeshRenderer::SetModel(Model* model)
{
	m_model = model;
	BoundingBox modelBB;
	modelBB.Set(model->GetBoundingBox());
	BB.Set(modelBB.Transform(GetModelMatrix()));
}

Shader * MeshRenderer::GetShader()
{
	return m_shader;
}

Model * MeshRenderer::GetModel()
{
	return m_model;
}
