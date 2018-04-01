#include "stdafx.h"
#include "Component.h"


Component::Component() : parent(nullptr)
{
	
}


Component::~Component()
{

}

void Component::Init()
{
}

void Component::Update()
{
}

void Component::Render()
{
}

void Component::SetParent(Composite * parent)
{
	this->parent = parent;
}

Composite * Component::GetParent()
{
	return parent;
}



