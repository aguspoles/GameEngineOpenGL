#include "stdafx.h"
#include "Game.h"
#include "InputHandler.h"
#include "Time.h"
#include "display.h"

bool Game::isRunning = true;

Game::Game() : _frameCounter(0), _frames(0), _timeSinceLastUpdate(0)
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
	for each(Shader* shader in m_shaders)
	{
		if (shader)
		{
			delete shader;
			shader = NULL;
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
		if (_frameCounter >= Time::SECOND) {
			_frames = 0;
			_frameCounter = 0;
		}
		Display::Instance()->Clear(0.0f, .0f, 0.0f, 1.0f);

		Root.Render();
		_frames++;
		ShowInfo();
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

	_frameCounter += _deltaTime;
	Time::deltaTime = _deltaTime;
}

void Game::SetFPSCapped(bool value)
{
	_fpsCapped = value;
}

void Game::ShowInfo()
{
	cout << "FPS:" << _frames << endl;
	cout << "Objects Rendered: " << Root.ObjectsRendered << endl;
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

void Game::AddShader(Shader * shader)
{
	m_shaders.push_back(shader);
}


