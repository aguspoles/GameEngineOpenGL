#include "stdafx.h"
#include "Composite.h"
#include "Mesh.h"
#include "MeshRenderer.h"

unsigned int Composite::ObjectsRendered = 0;
bool Composite::ShowAABB = false;

Composite::Composite() : BBShader(NULL)
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
	if (BBShader) delete BBShader;
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
	m_modelMatrix = transform->GetModelMatrix();

	Composite* parent = GetParent();
	if (parent)
	{
		m_modelMatrix = parent->GetModelMatrix() * m_modelMatrix;
		m_worldPosition = transform->position + parent->GetWorldPosition();
		m_worldRotation = transform->position + parent->GetWorldRotation();
	}
	MeshRenderer* meshRender = dynamic_cast<MeshRenderer*>(this);
	if (meshRender) {
		BBShader = new Shader("../res/basicShader");
		Model* model = meshRender->GetModel();
		for (size_t i = 0; i < model->meshes.size(); i++)
		{
			string name = model->meshes[i].name;
			meshesBBs[name].InitMesh();
		}
		allBB.InitMesh();
	}

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
	MeshRenderer* mr = dynamic_cast<MeshRenderer*>(this);
	if (mr) {
		Model* model = mr->GetModel();
		for (size_t i = 0; i < model->meshes.size(); i++)
		{
			string name = model->meshes[i].name;
			int posInFrustum = mr->camera->frustum.boxInFrustum(meshesBBs[name], mr->camera);
			if (posInFrustum == FrustumG::INSIDE || posInFrustum == FrustumG::INTERSECT) {
				RenderComposite(m_modelMatrix);
				if (ShowAABB == true) {
					meshesBBs[name].Render(BBShader, mr->camera);
					allBB.Render(BBShader, mr->camera);
					//cout << BBs[name].name << endl;
				}
				ObjectsRendered++;
			}
		}
	}
	else {
		RenderComposite(m_modelMatrix);
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
		allBB.Combine(composite->allBB);
		allBB.Refresh();
	}
	//bottom-up recalculation
	Composite* parent = GetParent();
	if (parent)
		parent->RecalculateBB(this);
}

void Composite::RemoveBB(Component * childComponent)
{
	//if i am a meshrenderer, take the original bounding box of the model and recalculate
	/*MeshRenderer* mesh = dynamic_cast<MeshRenderer*>(this);
	if (mesh) {
		BoundingBox modelBB = mesh->GetModel()->GetBoundingBox();
		BB.Set(modelBB);

		for (size_t i = 0; i < _components.size(); i++) {
			RecalculateBB(_components[i]);
		}

		Composite* parent = GetParent();
		if (parent)
			parent->RemoveBB(this);
	}*/
}

void Composite::TransformBB()
{
	//first we change our BB
	MeshRenderer* meshRender = dynamic_cast<MeshRenderer*>(this);
	BoundingBox bb;
	if (meshRender) {
		Model* model = meshRender->GetModel();
		for (size_t i = 0; i < model->meshes.size(); i++)
		{
			BoundingBox meshBB = model->meshes[i].BB;
			bb.Combine(meshBB);
			bb.Refresh();
			string name = meshBB.name;
			meshesBBs[name].Set(bb.Transform(meshRender->GetModelMatrix()));
		}
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
	if (GetParent()) {
		GetParent()->RecalculateBB(this);
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



