#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm\glm.hpp>
#include <iostream>

enum Mouse_Buttons {
	LEFT_CLICK = 0,
	MIDDEL_CLICK = 1,
	RIGHT_CLICK = 2
};

class ENGINE_API InputHandler
{
public:
	static InputHandler* Instance();
	~InputHandler();

	void CheckInput();
	void Update();
	bool GetMouseButtonState(int buttonNumber) const;
	glm::vec2 GetMousePos() const;
	glm::vec2 GetMouseWheel() const;

private:
	InputHandler();
	static InputHandler* m_instance;
	std::vector<bool> m_mouseButtonStates;
	glm::vec2 m_mousePos;
	glm::vec2 m_mouseWheel;
};
typedef InputHandler TheInputHandler;

#endif

