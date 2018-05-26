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
		transform->position.x -= 2 * Time::deltaTime;
	if (m_moveRight)
		transform->position.x += 2 * Time::deltaTime;
	if (m_moveUp)
		transform->position.y += 5 * Time::deltaTime;
	if (m_moveDown)
		transform->position.y -= 5 * Time::deltaTime;
	if (m_moveForward)
		transform->position.z += 5 * Time::deltaTime;
	if (m_moveBackward)
		transform->position.z -= 5 * Time::deltaTime;

	//transform->rotation.y = Time::GetTime() * 0.5f;

	transform->UpdateModelMatrix();
}

void Player::InitComposite()
{
	type = "player";
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
	transform->position.y = -1.0;
	transform->scale.x = 0.10;
	transform->scale.y = 0.10;
	transform->scale.z = 0.10;
	transform->UpdateModelMatrix();
}

void Player::SetShaderProperties()
{
	glm::vec3 lightPos = Application::light->transform->position;
	//glm::vec3 lightPos = glm::vec3(0);
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

void Player::HandleInput()
{
	m_moveLeft = TheInputHandler::Instance()->GetButtonState(LEFT_BUTTON);
	m_moveRight = TheInputHandler::Instance()->GetButtonState(RIGHT_BUTTON);
	m_moveUp = TheInputHandler::Instance()->GetButtonState(UP_BUTTON);
	m_moveDown = TheInputHandler::Instance()->GetButtonState(DOWN_BUTTON);
	m_moveForward = TheInputHandler::Instance()->GetButtonState(FORWARD_BUTTON);
	m_moveBackward = TheInputHandler::Instance()->GetButtonState(BACKWARD_BUTTON);
}
