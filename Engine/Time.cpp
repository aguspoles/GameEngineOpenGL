#include "stdafx.h"
#include "Time.h"

double Time::deltaTime = 0;
Time* Time::_instance = NULL;
float Time::SECOND = 1.0f;

Time::Time()
{
}


Time * Time::Instance()
{
	if (_instance)
		return _instance;
	return _instance = new Time();
}

Time::~Time()
{
	if (_instance)
		delete _instance;
}

double Time::GetTime()
{
	return glfwGetTime();
}
