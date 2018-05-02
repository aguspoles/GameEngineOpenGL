#include "stdafx.h"
#include "Composite.h"
#include "Mesh.h"
#include "MeshRenderer.h"

unsigned int Composite::ObjectsRendered = 0;

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
	if (dynamic_cast<MeshRenderer*>(this)) {
		BBShader = new Shader("../res/basicShader");
		BB.InitMesh();
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
	if (dynamic_cast<MeshRenderer*>(this)) {
		PositionInFrustum position = BoxInFrustum(BB);
		if (position == PositionInFrustum::INSIDE || position == PositionInFrustum::INTERSECT) {
			RenderComposite(m_modelMatrix);
			BB.InitMesh();
			BB.Render(BBShader);
			ObjectsRendered++;
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
		BB.Combine(composite->BB);
		BB.Refresh();
	}
	//bottom-up recalculation
	Composite* parent = GetParent();
	if (parent)
		parent->RecalculateBB(this);
}

PositionInFrustum Composite::BoxInFrustum(BoundingBox bb)
{
	unsigned int Out, In;
	PositionInFrustum res = PositionInFrustum::INSIDE;
	vector<Plane> planes = Camera::MainCamera->FrustumPlanes();
	vector<glm::vec3> verts;
	for (size_t i = 0; i < 8; i++)
	{
		verts.push_back(bb.vertices[i]);
		//verts[i] = glm::vec4(bb.vertices[i], 1.0) * Camera::MainCamera->GetViewMatrix() * Camera::MainCamera->GetProjectionMatrix();
	}
	// for each plane do ...
	for (size_t i = 0; i < 6; i++)
	{
		// reset counters for corners in and out
		Out = In = 0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (size_t k = 0; k < 8 && (In == 0 || Out == 0); k++)
		{
			// is the corner outside or inside
			if (planes[i].Distance(verts[k]) < 0)
				Out++;
			else
				In++;
		}
		//if all corners are out
		if (In == 0)
			return PositionInFrustum::OUTSIDE;
		// if some corners are out and others are in
		else if (Out > 0)
			res = PositionInFrustum::INTERSECT;
	}
	return(res);
}

void Composite::RemoveBB(Component * childComponent)
{
	//if i am a meshrenderer, take the original bounding box of the model and recalculate
	MeshRenderer* mesh = dynamic_cast<MeshRenderer*>(this);
	if (mesh) {
		BoundingBox modelBB = mesh->GetModel()->GetBoundingBox();
		BB.Set(modelBB);

		for (size_t i = 0; i < _components.size(); i++) {
			RecalculateBB(_components[i]);
		}

		Composite* parent = GetParent();
		if (parent)
			parent->RemoveBB(this);
	}
}

void Composite::TransformBB()
{
	//first we change our BB
	MeshRenderer* mesh = dynamic_cast<MeshRenderer*>(this);
	BoundingBox bb;
	if (mesh) {
		Model* model = mesh->GetModel();
		BoundingBox modelBB = model->GetBoundingBox();
		bb.Combine(modelBB);
		bb.Refresh();
		BB.Set(bb.Transform(mesh->GetModelMatrix()));
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



