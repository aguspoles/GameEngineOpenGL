#include "stdafx.h"
#include "display.h"
#include <iostream>

Display* Display::_instance = NULL;
unsigned int Display::SCR_WIDTH = 0;
unsigned int Display::SCR_HEIGHT = 0;
GLFWwindow* Display::window = NULL;
std::string Display::TITLE = "";

Display::Display(unsigned int width, unsigned int height, const std::string title)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	TITLE = title;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
}

Display* Display::Instance()
{
	if (_instance)
		return _instance;
	else if(SCR_WIDTH <= 0 || SCR_HEIGHT <= 0)
		return _instance = new Display(800, 600, "DefaultWindow");
	return _instance = new Display(SCR_WIDTH, SCR_HEIGHT, TITLE);
}

Display::~Display()
{
	if (_instance)
		delete _instance;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Display::Clear(float r, float g, float b, float a) const
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SwapBuffers() const
{
	glfwSwapBuffers(window);
}