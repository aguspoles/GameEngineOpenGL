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
	glm::vec3 viewPos = Camera::MainCamera->Position;

	m_shader->setMat4("model", this->GetModelMatrix());
	m_shader->setMat4("view", Camera::MainCamera->GetViewMatrix());
	m_shader->setMat4("projection", Camera::MainCamera->GetProjectionMatrix());

	m_shader->setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
	m_shader->setVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);

	m_shader->setFloat("material.shininess", 64);
	m_shader->setVec3("light.ambient", 1.0, 1.0, 1.0);
	m_shader->setVec3("light.diffuse", 1.0, 1.0, 1.0);
	m_shader->setVec3("light.specular", 1.0, 1.0, 1.0);
}

void Enemy::HandleInput()
{

}
