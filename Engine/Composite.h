#ifndef COMPOSITE_H
#define COMPOSITE_H
#include <vector>
#include "Component.h"
#include <glm/glm.hpp>
#include "Transform.h"
#include "Utils.h"

using namespace std;

class ENGINE_API Composite : public Component
{
public:
	Composite();
	virtual ~Composite();

	Transform* transform;
	static unsigned int ObjectsRendered;

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
	void Init() override final;
	void Update() override final;
	void Render() override final;
	template<class T> T* GetComponent();
	template<class T> T* GetComponentInChildren();
	template<class T> T* GetComponentInParent();
	template<class T> vector<T*> GetComponents();
	template<class T> vector<T*>* GetComponentsInParent();
	template<class T> vector<T*>* GetComponentsInChildren();

	glm::mat4 GetModelMatrix();
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldRotation();
	vector<Component*> GetComponents();

	BoundingBox BB;
	void TransformBB();
	void RecalculateBB(Component* childComponent);
	//if the BB is in frustum
	PositionInFrustum BoxInFrustum(BoundingBox bb);
	Shader* BBShader;

	string type;

protected:
	virtual void InitComposite();
	virtual void UpdateComposite();
	virtual void RenderComposite(glm::mat4 tempMatrix);
private:
	vector<Component*> _components;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_worldPosition;
	glm::vec3 m_worldRotation;

	template<class T> void GetComponentsInParent(vector<T*>*);
	template<class T> void GetComponentsInChildren(vector<T*>*);

	void RemoveBB(Component* childComponent);
};

template<class T>
inline T* Composite::GetComponent()
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(_components[i]);
		if (comp != nullptr) return comp;
	}

	return nullptr;
}

template<class T>
inline vector<T*> Composite::GetComponents()
{
	vector<T*> res;
	for (size_t i = 0; i < _components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(_components[i]);
		if (comp != nullptr) res.push_back(comp);
	}

	return res;
}

template<class T>
inline T * Composite::GetComponentInChildren()
{
	T* comp = dynamic_cast<T*>(this);
	if (comp) return comp;

	for (size_t i = 0; i < _components.size(); i++)
	{
		Component* child = _components[i];
		Composite* compositeChild = dynamic_cast<Composite*>(child);
		if (compositeChild)
		{
			T* childComp = compositeChild->GetComponentInChildren<T>();
			if (childComp) return childComp;
		}
		else
		{
			T* childComp = dynamic_cast<T*>(child);
			if (childComp) return childComp;
		}
	}

	return nullptr;
}

template<class T>
inline T * Composite::GetComponentInParent()
{
	T* comp = dynamic_cast<T*>(this);

	//Verifico si yo soy el tipo que esta buscando
	if (comp != nullptr)
	{
		return comp;
	}
	//Si no verifico si tengo padre para preguntarle a el
	else if (GetParent() != nullptr)
	{
		return GetParent()->GetComponentInParent<T>();
	}
	//Si no no esta lo que se esta buscando
	else
	{
		return nullptr;
	}
}

template<class T>
inline vector<T*>* Composite::GetComponentsInParent()
{
	vector<T*>* vec = new vector<T*>();
	GetComponentsInParent<T*>(vec);
	return vec;
}

template<class T>
inline vector<T*>* Composite::GetComponentsInChildren()
{
	vector<T*>* vec = new vector<T*>();
	GetComponentsInChildren<T*>(vec);
	return vec;
}

template<class T>
inline void Composite::GetComponentsInParent(vector<T*>* vec)
{
	T* comp = dynamic_cast<T*>(this);
	if (comp) vec->push_back(comp);
	if (GetParent()) GetParent()->GetComponentsInParent(vec);
}

template<class T>
inline void Composite::GetComponentsInChildren(vector<T*>* vec)
{
	T* comp = dynamic_cast<T*>(this);
	if (comp) vec->push_back(comp);
	if (GetChildren()) GetChildren()->GetComponentsInChildren(vec);
}


/* other posible non recursive implementation
template<class T>
inline vector<T*>* Composite::GetComponentsInParent()
{
vector<T*>* vec = new vector<T*>();
Composite* current = this;
while (current)
{
T* comp = dynamic_cast<T*>(current);
if (comp) vec->push_back(comp);
current = current->GetParent();
}
return vec;
}*/

#endif