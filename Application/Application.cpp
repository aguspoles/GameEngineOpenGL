#include "stdafx.h"
#include "Application.h"
#include "display.h"
#include "Time.h"
#include "InputHandler.h"
#include "Model.h"
#include "Shader.h"
#include "Player.h"
#include "Enemy.h"
#include "Platform.h"

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
	Camera::MainCamera->MouseSensitivity = 10;

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
	Model* cube = new Model("../res/cube.obj");

	ShadersHolder.LoadFromFile(Assets::Shader::Phong, "../res/lightShader");
	ShadersHolder.LoadFromFile(Assets::Shader::Basic, "../res/lightSource");
	ShadersHolder.LoadFromFile(Assets::Shader::BB, "../res/basicShader");

	Player* player = new Player;
	player->SetModel(nanosuit);
	player->SetShader(&ShadersHolder.GetResource(Assets::Shader::Phong));

	Enemy* enemy = new Enemy;
	enemy->SetModel(cube);
	enemy->SetShader(&ShadersHolder.GetResource(Assets::Shader::Basic));

	Platform* plat = new Platform;
	plat->SetModel(cube);
	plat->SetShader(&ShadersHolder.GetResource(Assets::Shader::Basic));

	light = new Light(lightVertices, lightIndices);
	light->SetShader(&ShadersHolder.GetResource(Assets::Shader::Basic));
	light->Color = glm::vec3(1.0, 0, 0);

	//AddShader("phong", phongShader);
	//AddShader("lightSource", basicShader);
	//AddShader("BB", BBShader);
	AddModel(nanosuit);

	player->AddComponent(plat);
	plat->AddComponent(enemy);
	Root.AddComponent(player);
	Root.AddComponent(light);
}

void Application::Update()
{

}

void Application::Render()
{
}
