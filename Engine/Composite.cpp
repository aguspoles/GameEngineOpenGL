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

	//RemoveBB(component);
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
	//first we change our BB
	MeshRenderer* meshRender = dynamic_cast<MeshRenderer*>(this);
	if (meshRender) {
		BoundingBox bb;
		//recalculate BB base on our original meshes BBs
		meshRender->CalculateBB();
		bb.Set(meshRender->BB);
		meshRender->BB.Set(bb.Transform(m_modelMatrix));
	}
	else {
		this->BB.Set(this->BB.Transform(m_modelMatrix));
	}
	/*if (GetParent())
		GetParent()->RecalculateBB(this);*/

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

	//first we change our BB
	MeshRenderer* meshRender = dynamic_cast<MeshRenderer*>(this);
	if (meshRender) {
		BoundingBox bb;
		//recalculate BB base on our original meshes BBs
		meshRender->CalculateBB();
		bb.Set(meshRender->BB);
		meshRender->BB.Set(bb.Transform(m_modelMatrix));
	}
	else {
		this->BB.Set(this->BB.Transform(m_modelMatrix));
	}
	/*if (GetParent())
		GetParent()->RecalculateBB(this);*/


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
				this->BB.Render(this->camera, BBshader);
			    cout << this->type << endl;
			}
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
		BoundingBox bb;
		bb.Combine(composite->BB);
		bb.Refresh();
		this->BB.Set(bb);
	}
	//bottom-up recalculation
	Composite* parent = GetParent();
	if (parent)
		parent->RecalculateBB(this);
}
//
//void Composite::RemoveBB(Component * childComponent)
//{
//	//if i am a meshrenderer, take the original bounding box of the model and recalculate
//	/*MeshRenderer* mesh = dynamic_cast<MeshRenderer*>(this);
//	if (mesh) {
//		BoundingBox modelBB = mesh->GetModel()->GetBoundingBox();
//		BB.Set(modelBB);
//
//		for (size_t i = 0; i < _components.size(); i++) {
//			RecalculateBB(_components[i]);
//		}
//
//		Composite* parent = GetParent();
//		if (parent)
//			parent->RemoveBB(this);
//	}*/
//}
//
void Composite::TransformBB()
{
	m_modelMatrix = transform->GetModelMatrix();
	Composite* parent = GetParent();
	if (parent)
		m_modelMatrix = parent->GetModelMatrix() * m_modelMatrix;

	//first we change our BB
	MeshRenderer* meshRender = dynamic_cast<MeshRenderer*>(this);
	if (meshRender) {
		BoundingBox bb;
		//recalculate BB base on our original meshes BBs
		meshRender->CalculateBB();
		bb.Set(meshRender->BB);
		meshRender->BB.Set(bb.Transform(meshRender->GetModelMatrix()));
	}
	else {
		this->BB.Set(this->BB.Transform(m_modelMatrix));
	}

	//then we change our children
	for (size_t i = 0; i < _components.size(); i++)
	{
		Composite* comp = dynamic_cast<Composite*>(_components[i]);
		if (comp) {
			comp->TransformBB();
		}
	}

	//finally we recalculate our parent's BB with ours and the recursion of RecalculateBB()
	//do the bottom-up caculations
	/*if (GetParent()) {
		GetParent()->RecalculateBB(this);
	}*/
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


