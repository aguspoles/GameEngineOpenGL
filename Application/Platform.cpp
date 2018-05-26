#include "stdafx.h"
#include "Platform.h"


Platform::Platform()
{
	type = "platform";
}


Platform::~Platform()
{
}

void Platform::UpdateComposite()
{
}

void Platform::InitComposite()
{
	transform->scale.x = 0.5;
	transform->scale.y = 0.5;
	transform->scale.z = 0.5;
	//transform->position.y = -1;
	transform->UpdateModelMatrix();
}

void Platform::SetShaderProperties()
{
	shader->setMat4("model", this->GetModelMatrix());
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());
}

void Platform::HandleInput()
{
}

