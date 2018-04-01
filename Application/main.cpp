#include "stdafx.h"
#include "Application.h"

int main() {
	Application* game = new Application;
	game->Run();

	delete game;
	return 0;
}