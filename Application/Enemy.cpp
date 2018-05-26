#include "stdafx.h"
#include "Enemy.h"
#include "Time.h"
#include "InputHandler.h"
#include "Application.h"

Enemy::Enemy()
{
	type = "enemy";
}


Enemy::~Enemy()
{
}

void Enemy::UpdateComposite()
{
	HandleInput();

	//transform->GetPos()->y -= 5 * Time::deltaTime;

	transform->rotation.y = Time::GetTime() * 0.5f;
	transform->rotation.x = Time::GetTime() * 0.5f;

	transform->UpdateModelMatrix();
}

void Enemy::InitComposite()
{
	transform->scale.x = 0.5;
	transform->scale.z = 0.5;
	transform->scale.y = 4;
	transform->position.y = 10;
	transform->position.x = 2;
	transform->UpdateModelMatrix();
}

void Enemy::SetShaderProperties()
{
	glm::vec3 lightPos = Application::light->transform->position;
	glm::vec3 viewPos = camera->Position;

	shader->setMat4("model", this->GetModelMatrix());
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectionMatrix());

	shader->setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
	shader->setVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);

	shader->setFloat("material.shininess", 64);
	shader->setVec3("light.ambient", 1.0, 1.0, 1.0);
	shader->setVec3("light.diffuse", 1.0, 1.0, 1.0);
	shader->setVec3("light.specular", 1.0, 1.0, 1.0);
}

void Enemy::HandleInput()
{

}
