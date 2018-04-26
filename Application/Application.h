#pragma once
#include "Game.h"
#include "Light.h"
#include "ResourceHolder.h"
#include "Resources.h"
#include "Shader.h"

class Application :
	public Game
{
public:
	Application();
	~Application();

	static Light* light;
	ResourceHolder<Shader, Assets::Shader::ID> ShadersHolder;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

};

