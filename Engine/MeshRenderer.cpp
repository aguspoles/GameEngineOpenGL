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
	meshesBBs.clear();
	m_model = model;
	for (size_t i = 0; i < m_model->meshes.size(); i++)
	{
		string name = m_model->meshes[i].name;
		BoundingBox meshBB;
		meshBB.Set(m_model->meshes[i].BB);
		allBB.Combine(meshBB);
		meshesBBs[name].Set(meshBB.Transform(this->GetModelMatrix()));
	}
	allBB.Refresh();
}

Shader * MeshRenderer::GetShader()
{
	return m_shader;
}

Model * MeshRenderer::GetModel()
{
	return m_model;
}
