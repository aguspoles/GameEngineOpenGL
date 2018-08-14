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

	std::vector<MeshRenderer*> GetAllMeshRenderers(std::vector<MeshRenderer*> entities);

public:
	static BSP * Instance();
	~BSP();
	std::vector<Plane> Planes;
	std::vector<MeshRenderer*> objects;
	std::vector<MeshRenderer*> FilterBaseOnCamera(Camera* camera);
};

