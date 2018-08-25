#include "stdafx.h"
#include "BSP.h"
#include "Game.h"
#include "InputHandler.h"

BSP* BSP::m_instance = NULL;

BSP::BSP()
{
}

void BSP::GetAllComposites(std::vector<Component*>* components, std::vector<Composite*>& objects)
{
	for (size_t i = 0; i < components->size(); i++)
	{
		if (Composite* comp = dynamic_cast<Composite*>(components->at(i)))
		{
			objects.push_back(comp);
			GetAllComposites(&comp->GetComponents(), objects);
		}
	}
}

BSP::~BSP()
{
}

BSP * BSP::Instance()
{
	if (m_instance == NULL) {
		m_instance = new BSP();
	}
	return m_instance;
}

std::vector<Composite*> BSP::FilterBaseOnCamera(Camera * camera)
{
	std::vector<Composite*> res; res.clear();
	GetAllComposites(&Game::GetEntities(), res);
	std::vector<Composite*> objectsToFilter;

	for (size_t i = 0; i < Planes.size(); i++)
	{
		for (size_t j = 0; j < res.size(); j++)
		{
			float objectDistanceMin = Planes[i].distance(res[j]->BB.GetMin());
			float objectDistanceMax = Planes[i].distance(res[j]->BB.GetMax());

			float cameraDistance = Planes[i].distance(camera->Position);
	
			if(cameraDistance * objectDistanceMin > 0 || cameraDistance * objectDistanceMax > 0)
				objectsToFilter.push_back(res[j]);
		}
		res = objectsToFilter;
		objectsToFilter.clear();
	}

	//objects.clear();
	return res;
}


