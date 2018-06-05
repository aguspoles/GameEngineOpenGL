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

	Camera::MainCamera = new Camera(glm::vec3(0.0f, 0.0f, 6.0f));
	Camera::MainCamera->MouseSensitivity = 10;

	TheInputHandler::Instance()->camera = Camera::MainCamera;

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

	//Model* nanosuit = new Model("../res/nanosuit/nanosuit.obj");
	Model* room = new Model("../res/room.obj");
	//Model* cube = new Model("../res/cube.obj");

	ShadersHolder.LoadFromFile(Assets::Shader::Phong, "../res/lightShader");
	ShadersHolder.LoadFromFile(Assets::Shader::Basic, "../res/lightSource");
	ShadersHolder.LoadFromFile(Assets::Shader::BB, "../res/basicShader");

	Player* player = new Player;
	player->SetModel(room);
	player->SetShader(&ShadersHolder.GetResource(Assets::Shader::Phong));

	/*Enemy* enemy = new Enemy;
	enemy->SetModel(cube);
	enemy->SetShader(&ShadersHolder.GetResource(Assets::Shader::Basic));*/

	Platform* plat = new Platform();
	plat->SetModel(room);
	plat->SetShader(&ShadersHolder.GetResource(Assets::Shader::Basic));

	light = new Light(lightVertices, lightIndices);
	light->SetShader(&ShadersHolder.GetResource(Assets::Shader::Basic));
	light->Color = glm::vec3(1.0, 0, 0);

	//AddShader("phong", phongShader);
	//AddShader("lightSource", basicShader);
	//AddShader("BB", BBShader);
	//AddModel(nanosuit);
	//AddModel(cube);
	AddModel(room);

	//player->AddComponent(enemy);
	//plat->AddComponent(enemy);
    Root.AddComponent(light);
	Root.AddComponent(plat);
	//enemy->AddComponent(player);
	//Root.AddComponent(player);
	Root.type = "root";
	Root.SetCamera(Camera::MainCamera);
	Root.BB.name = "root";
	player->BB.name = "player";
}

void Application::Update()
{
	if (TheInputHandler::Instance()->GetButtonState(AABB_BUTTON))
		Root.ShowAABB = true;
	else
		Root.ShowAABB = false;
}

void Application::Render()
{
	if (Root.ShowAABB) {
		//Camera::MainCamera->frustum.drawPoints(&ShadersHolder.GetResource(Assets::Shader::Basic), Camera::MainCamera);
		Camera::MainCamera->frustum.drawNormals(&ShadersHolder.GetResource(Assets::Shader::Basic), Camera::MainCamera);
	}
}
