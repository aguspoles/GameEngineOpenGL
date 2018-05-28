#include "stdafx.h"
#include "MeshRenderer.h"
#include "Camera.h"


MeshRenderer::MeshRenderer() : shader(NULL)
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
	shader->use();

	shader->setMat4("model", this->GetModelMatrix());
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader);
	}
}

void MeshRenderer::CalculateBB()
{
	BoundingBox bb;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		bb.Combine(meshes[i].BB);
	}
	bb.Refresh();
	this->BB.Set(bb);
}


