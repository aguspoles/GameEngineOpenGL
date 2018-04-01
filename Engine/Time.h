#ifndef TIME_H
#define TIME_H
#include <GLFW\glfw3.h>

class ENGINE_API Time
{
public:
	static Time* Instance();
	static float SECOND;
	~Time();

	static double GetTime();
	static double deltaTime;
private:
	static Time* _instance;
	Time();
};

#endif
