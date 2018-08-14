#include "stdafx.h"
#include "BSP.h"

template <class T>
inline int sgn(T v)
{
	return (v > T(0)) - (v < T(0));
}

BSP* BSP::m_instance = NULL;

BSP::BSP()
{
}

std::vector<MeshRenderer*> BSP::GetAllMeshRenderers(std::vector<MeshRenderer*> entities)
{
	std::vector<MeshRenderer*> res;
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (MeshRenderer* mr = dynamic_cast<MeshRenderer*>(entities[i]))
			res.push_back(mr);
		for (size_t j = 0; j < entities[i]->GetComponents().size(); j++)
		{
			Component* c = entities[i]->GetComponents()[j];
			if (MeshRenderer* mr = dynamic_cast<MeshRenderer*>(c))
				res.push_back(mr);
		}
	}
	return res;
}


BSP * BSP::Instance()
{
	if (m_instance == NULL) {
		m_instance = new BSP();
	}
	return m_instance;
}

BSP::~BSP()
{
}

std::vector<MeshRenderer*> BSP::FilterBaseOnCamera(Camera * camera)
{
	std::vector<MeshRenderer*> res;
	std::vector<MeshRenderer*> objectsToFilter;
	res = objects;
	for (size_t i = 0; i < Planes.size(); i++)
	{
		for (size_t j = 0; j < res.size(); j++)
		{
			//int objectDistance = Planes[i].distance(objects[j]->transform->position);
			int objectDistanceMin = Planes[i].distance(res[j]->BB.GetMin());
			int objectDistanceMax = Planes[i].distance(res[j]->BB.GetMax());

			int cameraDistance = Planes[i].distance(camera->Position);
			/*if (sgn(objectDistance) == sgn(cameraDistance))
			{
				objectsToFilter.push_back(objects[j]);
			}*/
			if(cameraDistance * objectDistanceMin > 0 || cameraDistance * objectDistanceMax > 0)
				objectsToFilter.push_back(res[j]);
		}
		res = objectsToFilter;
		objectsToFilter.clear();
	}

	return res;
}


