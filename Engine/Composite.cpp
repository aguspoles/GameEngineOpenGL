#include "stdafx.h"
#include "Composite.h"
#include "Mesh.h"


Composite::Composite()
{
	transform = new Transform;
	AddComponent(transform);
	m_modelMatrix = transform->GetModelMatrix();
	m_worldPosition = *transform->GetPos();
	m_worldRotation = *transform->GetRot();
}

Composite::~Composite()
{
	if (transform)
		delete transform;
}

void Composite::AddComponent(Component * component)
{
	m_components.push_back(component);
	component->SetParent(this);
}

void Composite::RemoveComponent(Component * component)
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		if (m_components[i] == component) {
			delete component;
			m_components.erase(m_components.begin() + i);
		}
	}
}

void Composite::Init()
{
	InitComposite();
	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Init();
	}
}

void Composite::Update()
{
	m_modelMatrix = transform->GetModelMatrix();
	UpdateComposite();

	Composite* parent = GetParent();
	if (parent)
	{
		m_modelMatrix *= parent->GetModelMatrix();
		m_worldPosition = *transform->GetPos() + parent->GetWorldPosition();
		m_worldRotation = *transform->GetRot() + parent->GetWorldRotation();
	}

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update();
	}
}

void Composite::Render()
{
	RenderComposite(m_modelMatrix);

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render();
	}
}

void Composite::InitComposite()
{
}

void Composite::UpdateComposite()
{
}

void Composite::RenderComposite(glm::mat4 tempMatrix)
{
}

glm::mat4 Composite::GetModelMatrix()
{
	return m_modelMatrix;
}

glm::vec3 Composite::GetWorldPosition()
{
	return m_worldPosition;
}

glm::vec3 Composite::GetWorldRotation()
{
	return m_worldRotation;
}

vector<Component*> Composite::GetComponents()
{
	return m_components;
}



