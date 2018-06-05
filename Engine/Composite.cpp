#include "stdafx.h"
#include "Composite.h"
#include "Mesh.h"
#include "ModelRenderer.h"

unsigned int Composite::ObjectsRendered = 0;
bool Composite::ShowAABB = false;
Shader* Composite::BBshader = NULL;

Composite::Composite() : camera(NULL)
{
	transform = new Transform;
	AddComponent(transform);
	m_modelMatrix = transform->GetModelMatrix();
	m_worldPosition = transform->position;
	m_worldRotation = transform->rotation;
}

Composite::~Composite()
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		if (_components[i])
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
}

void Composite::Init()
{
	InitComposite();
	m_modelMatrix = transform->GetModelMatrix();

	Composite* parent = GetParent();
	if (parent)
	{
		m_modelMatrix = parent->GetModelMatrix() * m_modelMatrix;
		m_worldPosition = transform->position + parent->GetWorldPosition();
		m_worldRotation = transform->position + parent->GetWorldRotation();
	}

	if (BBshader == NULL)
		BBshader = new Shader("../res/basicShader");
	this->BB.InitMesh();

	TransformBB();

	for (size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->Init();
	}

}

void Composite::Update()
{
	UpdateComposite();
	m_modelMatrix = transform->GetModelMatrix();

	Composite* parent = GetParent();
	if (parent)
	{
		m_modelMatrix = parent->GetModelMatrix() * m_modelMatrix;
		m_worldPosition = transform->position + parent->GetWorldPosition();
		m_worldRotation = transform->rotation + parent->GetWorldRotation();
	}

	TransformBB();

	for (size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->Update();
	}

}

void Composite::Render()
{
	if (this->camera) {
		int posInFrustum = this->camera->frustum.boxInFrustum(this->BB, this->camera);
		if (posInFrustum == FrustumG::INSIDE || posInFrustum == FrustumG::INTERSECT) {
			RenderComposite(m_modelMatrix);
			if (ShowAABB == true) {
				this->BB.InitMesh();
				this->BB.Render(this->camera, BBshader);
				if(dynamic_cast<MeshRenderer*>(this))
			       cout << this->type << endl;
			}
			if (dynamic_cast<MeshRenderer*>(this))
			   ObjectsRendered++;
		}
	}

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
	Composite* composite = dynamic_cast<Composite*>(childComponent);
	if (composite)
	{
		this->BB.Combine(composite->BB);
		this->BB.Refresh();
	}
	//bottom-up recalculation
	Composite* parent = GetParent();
	if (parent)
		parent->RecalculateBB(this);
}

void Composite::TransformBB()
{
	//first we change our BB
	MeshRenderer* meshRender = dynamic_cast<MeshRenderer*>(this);
	BoundingBox bb;
	if (meshRender) {
		//recalculate BB base on our original meshes BBs
		meshRender->CalculateBB();
		bb.Set(meshRender->BB);
		meshRender->BB.Set(bb.Transform(m_modelMatrix));
	}
	else {
		this->BB.Set(bb.Transform(m_modelMatrix));
	}
	if (GetParent())
		GetParent()->RecalculateBB(this);
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

void Composite::SetCamera(Camera * camera)
{
	this->camera = camera;
	SetCameraInChildren(this);
}

void Composite::SetCameraInChildren(Composite * comp)
{
	for (size_t i = 0; i < comp->_components.size(); i++)
	{
		if (Composite* compos = dynamic_cast<Composite*>(comp->_components[i])) {
			compos->camera = this->camera;
			SetCameraInChildren(compos);
		}
	}
}


