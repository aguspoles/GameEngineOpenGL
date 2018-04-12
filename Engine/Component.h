#ifndef COMPONENT_H
#define COMPONENT_H
#include "BoundingBox.h"

class ENGINE_API Component
{
private:
	class Composite* parent;
public:
	Component();
	virtual ~Component();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	void SetParent(Composite* parent);
	Composite* GetParent();
	BoundingBox BB;
protected:
};

#endif