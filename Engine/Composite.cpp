#include "stdafx.h"
#include "Composite.h"
#include "Mesh.h"
#include "MeshRenderer.h"


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
	for (size_t i = 0; i < _components.size(); i++)
	{
		if(_components[i])
			delete _components[i];
	}
}

void Composite::AddComponent(Component * component)
{
	_components.push_back(component);
	component->SetParent(this);
	RecalculateBB(component);
}

void Composite::RemoveComponent(Component * component)
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		if (_components[i] == component) {
			delete _components[i];
			_components.erase(_components.begin() + i);
		}
	}

	RemoveBB(component);
}

void Composite::Init()
{
	InitComposite();
	for (size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->Init();
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

	for (size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->Update();
	}
}

void Composite::Render()
{
	RenderComposite(m_modelMatrix);

	transform->GetBoundingBox()->ModelMatrix = m_modelMatrix;
	transform->GetBoundingBox()->Render();

	for (size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->Render();
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

void Composite::RecalculateBB(Component* childComponent)
{
	Composite* compositeChild = dynamic_cast<Composite*>(childComponent);
	if (compositeChild) {
		BoundingBox* childBB = compositeChild->transform->GetBoundingBox();
		transform->GetBoundingBox()->Combine(*childBB);
		transform->GetBoundingBox()->Refresh();
	}
	//bottom-up recalculation
	Composite* parent = GetParent();
	if (parent)
		parent->RecalculateBB(this);
}

void Composite::RemoveBB(Component * childComponent)
{
    //if i am a meshrenderer, take the original bounding box of the model and recalculate
	MeshRenderer* mesh = dynamic_cast<MeshRenderer*>(this);
	if (mesh) {
		BoundingBox modelBB = mesh->GetModel()->GetBoundingBox();
		transform->SetBoundingBox(modelBB);

		for (size_t i = 0; i < _components.size(); i++) {
			RecalculateBB(_components[i]);
		}

		Composite* parent = GetParent();
		if (parent)
			parent->RemoveBB(this);
	}
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
	return _components;
}



