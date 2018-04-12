#include "stdafx.h"
#include "Application.h"
#include "display.h"
#include "Time.h"
#include "InputHandler.h"
#include "Model.h"
#include "Shader.h"
#include "Player.h"

Light* Application::light = NULL;

Application::Application()
{
}


Application::~Application()
{
}

void Application::Init() {
	Display::SCR_WIDTH = 800;
	Display::SCR_HEIGHT = 600;
	Display::TITLE = "Engine";
	//instantiate display
	Display::Instance();
	//instantiate time
	Time::Instance();
	//instantiate inputHandler
	TheInputHandler::Instance();

	Camera::MainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	vector<unsigned int> lightIndices = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};
	std::vector<glm::vec3> lightVertices = {
		glm::vec3(-1, -1, -1),
		glm::vec3(-1, 1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(1, -1, -1),

		glm::vec3(-1, -1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, -1, 1),

		glm::vec3(-1, -1, -1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, -1, -1),

		glm::vec3(-1, 1, -1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, 1, -1),

		glm::vec3(-1, -1, -1),
		glm::vec3(-1, -1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, 1, -1),

		glm::vec3(1, -1, -1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, 1, -1)
	};

	Model* nanosuit = new Model("../res/nanosuit/nanosuit.obj");
	Shader* phongShader = new Shader("../res/lightShader.vs", "../res/lightShader.fs");
	Shader* basicShader = new Shader("../res/lightSource.vs", "../res/lightSource.fs");

	Player* player = new Player;
	player->SetModel(nanosuit);
	player->SetShader(phongShader);
	//test bounding box
	BoundingBox bb = player->BB;

	light = new Light(lightVertices, lightIndices);
	light->SetShader(basicShader);
	light->Color = glm::vec3(1.0, 0, 0);

	AddShader(phongShader);
	AddShader(basicShader);
	AddModel(nanosuit);

	Root.AddComponent(light);
	Root.AddComponent(player);
	Root.Init();
}

void Application::Update()
{
}

void Application::Render()
{
}
