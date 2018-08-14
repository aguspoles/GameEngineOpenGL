#include "stdafx.h"
#include "Game.h"
#include "InputHandler.h"
#include "Time.h"
#include "display.h"
#include "BSP.h"

bool Game::isRunning = true;
std::vector<Component*> Game::m_entities = {};

Game::Game() :  _frames(0), _timeSinceLastUpdate(0)
{
	_timePerFrame = 1.0f / FRAME_CAP;//time to render 1 frame
	_fpsCapped = false;
}

Game::~Game()
{
	for each(Mesh* mesh in m_meshes)
	{
		if (mesh)
		{
			delete mesh;
			mesh = NULL;
		}
	}
	for each(Texture* texture in m_textures)
	{
		if (texture)
		{
			delete texture;
			texture = NULL;
		}
	}
	for each(pair<string, Shader*> shader in m_shaders)
	{
		if (shader.second)
		{
			delete shader.second;
			shader.second = NULL;
		}
	}
	for each(Model* model in m_models)
	{
		if (model)
		{
			delete model;
			model = NULL;
		}
	}
	m_meshes.clear();
	m_shaders.clear();
	m_textures.clear();
	m_models.clear();
}

void Game::Run()
{
	Init();
	Root.Init();

	_lastFrameTime = Time::GetTime();
	double lastTime = Time::GetTime();

	while (!glfwWindowShouldClose(Display::window))
	{
		ManageTime();

		if (_fpsCapped)
		{
			while (_timeSinceLastUpdate > _timePerFrame)
			{
				_timeSinceLastUpdate -= _timePerFrame;

				HandleEvents();
				Update();
				Root.Update();

				if (glfwWindowShouldClose(Display::window))
					continue;
			}
		}
		else
		{
			HandleEvents();
			Update();
			Root.Update();
		}
	
		Display::Instance()->Clear(0.0f, .0f, 0.0f, 1.0f);

		Render();
		Root.Render();

		_frames++;
		//show info once every half second
		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= Time::SECOND/2) {
			ShowInfo();
			lastTime += 1.0;
			_frames = 0;
		}
		Root.ObjectsRendered = 0;

		Display::Instance()->SwapBuffers();
		glfwPollEvents();
	}
}

void Game::HandleEvents()
{
	TheInputHandler::Instance()->Update();
}

void Game::ManageTime()
{
	_currentFrameTime = Time::GetTime();
	_deltaTime = _currentFrameTime - _lastFrameTime;
	_lastFrameTime = _currentFrameTime;
	_timeSinceLastUpdate += _deltaTime / Time::SECOND;

	Time::deltaTime = _deltaTime;
}

void Game::SetFPSCapped(bool value)
{
	_fpsCapped = value;
}

void Game::ShowInfo()
{
	//ms needed to render 1 frame
	cout << "ms/frame:" << 1000.0/double(_frames) << endl;
	cout << "Objects Rendered: " << Root.ObjectsRendered << endl;
	cout <<  "BSP planes: " << BSP::Instance()->Planes.size() << endl;
	for (size_t i = 0; i < BSP::Instance()->Planes.size(); i++)
	{
		cout << "Distance plane/camera pos" << endl;
		cout << BSP::Instance()->Planes[i].distance(Camera::MainCamera->Position) << endl;
	}
	cout << "--------------------" << endl;
}

void Game::AddMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
}

void Game::AddModel(Model * model)
{
	m_models.push_back(model);
}

void Game::AddTexture(Texture * texture)
{
	m_textures.push_back(texture);
}

void Game::AddShader(string name, Shader * shader)
{
	m_shaders[name] = shader;
}

void Game::AddEntitite(Composite * comp)
{
	m_entities.push_back(comp);
}

std::vector<Component*> Game::GetEntities()
{
	return m_entities;
}

Shader * Game::GetShader(string name)
{
	return m_shaders[name];
}


