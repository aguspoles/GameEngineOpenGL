#pragma once
#include<vector>
#include "Plane.h"
#include "Camera.h"
#include "MeshRenderer.h"

class ENGINE_API BSP
{

private:
	static BSP * m_instance;
	BSP();

	void GetAllComposites(std::vector<Component*>* composites, std::vector<Composite*>& objects);

public:
	static BSP * Instance();
	~BSP();
	std::vector<Plane> Planes;
	//std::vector<Composite*> objects;
	std::vector<Composite*> FilterBaseOnCamera(Camera* camera);
};

