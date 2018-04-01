#include "stdafx.h"
#include "Player.h"
#include "Time.h"
#include "InputHandler.h"
#include "Application.h"


Player::Player() : m_moveLeft(false), m_moveRight(false)
{
}


Player::~Player() 
{
}

void Player::UpdateComposite()
{
	HandleInput();

	if (m_moveLeft)
		transform->GetPos()->x -= 5 * Time::deltaTime;
	if (m_moveRight)
		transform->GetPos()->x += 5 * Time::deltaTime;

	transform->GetRot()->y = Time::GetTime() * 0.5f;
	//GetTransform()->GetRot()->z = counter * Game::DeltaTime() * 50;
	//GetTransform()->GetPos()->z = -10;
	transform->GetPos()->y = -1.0;
	transform->GetScale()->x = 0.15;
	transform->GetScale()->y = 0.15;
	transform->GetScale()->z = 0.15;
	transform->UpdateModelMatrix();
}

void Player::InitComposite()
{
	m_shader->use();
	//gold
	/*glUniform3f(glGetUniformLocation(m_shader->ID, "material.ambient"), 0.24725, 0.1995, 0.0745);
	glUniform3f(glGetUniformLocation(m_shader->ID, "material.diffuse"), 0.75164, 0.60648, 0.22648);
	glUniform3f(glGetUniformLocation(m_shader->ID, "material.specular"), 0.628281, 0.555802, 0.366065);*/
	//cyan plastic
	/*glUniform3f(glGetUniformLocation(m_shader->ID, "material.ambient"), 0.0f, 0.1f, 0.06f);
	glUniform3f(glGetUniformLocation(m_shader->ID, "material.diffuse"), 0.0f, 0.50980392f, 0.50980392f);
	glUniform3f(glGetUniformLocation(m_shader->ID, "material.specular"), 0.50196078f, 0.50196078f, 0.50196078f);*/
	//cyan rubber
	//glUniform3f(glGetUniformLocation(m_shader->ID, "material.ambient"), 0.0, 0.05, 0.05);
	//glUniform3f(glGetUniformLocation(m_shader->ID, "material.diffuse"), 0.4, 0.5, 0.5);
	//glUniform3f(glGetUniformLocation(m_shader->ID, "material.specular"), 0.04, 0.7, 0.7);

	m_shader->setFloat("material.shininess", 64);
	m_shader->setFloat3("light.ambient", 1.0, 1.0, 1.0);
	m_shader->setFloat3("light.diffuse", 1.0, 1.0, 1.0);
	m_shader->setFloat3("light.specular", 1.0, 1.0, 1.0);
}

void Player::SetShaderProperties()
{
	glm::vec3 lightPos = *Application::light->transform->GetPos();
	glm::vec3 viewPos = Camera::MainCamera->Position;

	m_shader->setFloat3("light.position", lightPos.x, lightPos.y, lightPos.z);
	m_shader->setFloat3("viewPos", viewPos.x, viewPos.y, viewPos.z);
}

void Player::HandleInput()
{
	m_moveLeft = TheInputHandler::Instance()->GetMouseButtonState(LEFT_CLICK);
	m_moveRight = TheInputHandler::Instance()->GetMouseButtonState(RIGHT_CLICK);
}
