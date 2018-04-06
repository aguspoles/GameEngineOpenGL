#include "stdafx.h"
#include "transform.h"
#include "MeshRenderer.h"

Transform::Transform()
{
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rot = glm::vec3(0.0f, 0.0f, 0.0f);
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_modelMatrix = glm::mat4(1.0f);
}

Transform::Transform(const glm::vec3 & pos, const glm::vec3 & rot, const glm::vec3 & scale)
{
	_pos = pos;
	_rot = rot;
	_scale = scale;
	UpdateModelMatrix();
}

Transform::~Transform()
{
}

glm::mat4 Transform::UpdateModelMatrix()
{
	glm::mat4 posMat = glm::translate(_pos);
	glm::mat4 scaleMat = glm::scale(_scale);
	glm::mat4 rotX = glm::rotate(_rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(_rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(_rot.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 rotMat = rotX * rotY * rotZ;

	_modelMatrix = posMat * rotMat * scaleMat;
	return _modelMatrix;
}

BoundingBox * Transform::GetBoundingBox()
{
	return &_bb;
}

void Transform::SetBoundingBox(BoundingBox bb)
{
	_bb = bb;
}

void Transform::TransformBB()
{
	Composite* parent = GetParent();
	BoundingBox BB;
	if (parent) {
		MeshRenderer* parentMesh = dynamic_cast<MeshRenderer*>(parent);
		if (parentMesh) {
			Model* parentModel = parentMesh->GetModel();
			BoundingBox parentModelBB = parentModel->GetBoundingBox();
			BB.Combine(parentModelBB);
			BB.Refresh();
			BB.Transform(_modelMatrix);
		}

		vector<Component*> parentComponents = parent->GetComponents();
		for (size_t i = 0; i < parentComponents.size(); i++)
		{
			Composite* comp = dynamic_cast<Composite*>(parentComponents[i]);
			if (comp)
				comp->transform->TransformBB();
		}


		BB.Refresh();
		_bb = BB;

		for (size_t i = 0; i < parentComponents.size(); i++)
		{
			parent->RecalculateBB(parentComponents[i]);
		}
	}
}
