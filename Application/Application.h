#pragma once
#include "Game.h"
#include "Light.h"

class Application :
	public Game
{
public:
	Application();
	~Application();

	static Light* light;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

};

