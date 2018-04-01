#ifndef DISPLAY_H
#define DISPLAY_H

#include <glad/glad.h>
#include <string>
#include <GLFW\glfw3.h>

class ENGINE_API Display
{
public:
	static Display* Instance();
	virtual ~Display();

	static GLFWwindow* window;
	static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
	static std::string TITLE;

	void Clear(float r, float g, float b, float a) const;
	void SwapBuffers() const;

private:
	static Display* _instance;
	Display(unsigned int width, unsigned int height, const std::string title);
};

#endif
