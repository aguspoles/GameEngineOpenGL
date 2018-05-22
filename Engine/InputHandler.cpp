#include "stdafx.h"
#include "InputHandler.h"
#include "display.h"
#include "Time.h"
#include "Camera.h"

bool firstMouse = true;
float lastX = 400;
float lastY = 300;

InputHandler* InputHandler::m_instance = nullptr;
Camera* InputHandler::camera = NULL;
//util functions from glfw
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


InputHandler * InputHandler::Instance()
{
	if (!m_instance)
		m_instance = new InputHandler();
	return m_instance;
}

InputHandler::InputHandler() {
	for (size_t i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);
	}
	for (size_t i = 0; i < 7; i++)
	{
		m_ButtonStates.push_back(false);
	}
	glfwSetFramebufferSizeCallback(Display::window, framebuffer_size_callback);
	glfwSetCursorPosCallback(Display::window, mouse_callback);
	glfwSetScrollCallback(Display::window, scroll_callback);
}

InputHandler::~InputHandler()
{
	if (m_instance)
		delete m_instance;
}

void InputHandler::CheckInput()
{
}

void InputHandler::Update()
{
	processInput(Display::window);
	if (glfwGetKey(Display::window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_ButtonStates[RIGHT_BUTTON] = true;
	if (glfwGetKey(Display::window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_ButtonStates[LEFT_BUTTON] = true;
	if (glfwGetKey(Display::window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		m_ButtonStates[LEFT_BUTTON] = false;
	if (glfwGetKey(Display::window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		m_ButtonStates[RIGHT_BUTTON] = false;
	if (glfwGetKey(Display::window, GLFW_KEY_UP) == GLFW_PRESS)
		m_ButtonStates[UP_BUTTON] = true;
	if (glfwGetKey(Display::window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_ButtonStates[DOWN_BUTTON] = true;
	if (glfwGetKey(Display::window, GLFW_KEY_UP) == GLFW_RELEASE)
		m_ButtonStates[UP_BUTTON] = false;
	if (glfwGetKey(Display::window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		m_ButtonStates[DOWN_BUTTON] = false;
	if (glfwGetKey(Display::window, GLFW_KEY_1) == GLFW_PRESS)
		m_ButtonStates[FORWARD_BUTTON] = true;
	if (glfwGetKey(Display::window, GLFW_KEY_1) == GLFW_RELEASE)
		m_ButtonStates[FORWARD_BUTTON] = false;
	if (glfwGetKey(Display::window, GLFW_KEY_2) == GLFW_PRESS)
		m_ButtonStates[BACKWARD_BUTTON] = true;
	if (glfwGetKey(Display::window, GLFW_KEY_2) == GLFW_RELEASE)
		m_ButtonStates[BACKWARD_BUTTON] = false;
	if (glfwGetKey(Display::window, GLFW_KEY_KP_5) == GLFW_PRESS) {
			m_ButtonStates[AABB_BUTTON] = true;
	}
	if (glfwGetKey(Display::window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		m_ButtonStates[AABB_BUTTON] = false;
	}
}

bool InputHandler::GetMouseButtonState(int buttonNumber) const
{
	return m_mouseButtonStates[buttonNumber];
}

bool InputHandler::GetButtonState(int buttonNumber) const
{
	return m_ButtonStates[buttonNumber];
}

glm::vec2 InputHandler::GetMousePos() const
{
	return m_mousePos;
}

glm::vec2 InputHandler::GetMouseWheel() const
{
	return m_mouseWheel;
}

//------------------------------------------------------------------------------//

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		InputHandler::camera->ProcessKeyboard(FORWARD, Time::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		InputHandler::camera->ProcessKeyboard(BACKWARD, Time::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		InputHandler::camera->ProcessKeyboard(LEFT, Time::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		InputHandler::camera->ProcessKeyboard(RIGHT, Time::deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	InputHandler::camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler::camera->ProcessMouseScroll(yoffset);
}

