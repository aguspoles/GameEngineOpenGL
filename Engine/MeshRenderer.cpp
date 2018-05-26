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

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader);
	}
}


