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

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*m_shader);
	}
}

void MeshRenderer::SetShaderProperties()
{
}

void MeshRenderer::SetShader(Shader* shader)
{
	m_shader = shader;
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	/*m_mesh = mesh;
	BoundingBox meshBB;
	meshBB.Set(m_mesh->BB);
    this->BB.Set(meshBB.Transform(this->GetModelMatrix()));*/
}

Shader * MeshRenderer::GetShader()
{
	return m_shader;
}

