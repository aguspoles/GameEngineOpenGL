#include "stdafx.h"
#include "Enemy.h"
#include "Time.h"
#include "InputHandler.h"
#include "Application.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::UpdateComposite()
{
	HandleInput();

	/*if (m_moveLeft)
		transform->GetPos()->x -= 5 * Time::deltaTime;
	if (m_moveRight)
		transform->GetPos()->x += 5 * Time::deltaTime;*/

	//transform->GetRot()->y = Time::GetTime() * 0.5f;

	//transform->UpdateModelMatrix();
}

void Enemy::InitComposite()
{
	transform->GetScale()->x = 4;
	transform->GetScale()->y = 4;
	transform->GetScale()->z = 4;
	transform->UpdateModelMatrix();
}

void Enemy::SetShaderProperties()
{
	glm::vec3 lightPos = *Application::light->transform->GetPos();
	glm::vec3 viewPos = Camera::MainCamera->Position;

	m_shader->setMat4("model", GetModelMatrix());
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
	m_moveLeft = TheInputHandler::Instance()->GetMouseButtonState(LEFT_CLICK);
	m_moveRight = TheInputHandler::Instance()->GetMouseButtonState(RIGHT_CLICK);
}
