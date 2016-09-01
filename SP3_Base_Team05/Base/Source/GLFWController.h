#ifndef GLFWCONTROLLER_H
#define GLFWCONTROLLER_H
#include "GLFW\glfw3.h"

class GLFWController
{
private:

public:

	enum CONTROLLER_STICKS
	{
		L_THUMBSTICK_X,
		L_THUMBSTICK_Y,
		TRIGGERS,
		R_THUMBSTICK_Y,
		R_THUMBSTICK_X
	};
	enum CONTROLLER_BUTTON
	{
		A,
		B,
		X,
		Y,
		L_SHOULDER,
		R_SHOULDER,
		BACK,
		START,
		L_THUMBSTICK_PRESS,
		R_THUMBSTICK_PRESS,
		DPAD_UP,
		DPAD_RIGHT,
		DPAD_DOWN,
		DPAD_LEFT
	};

	GLFWController();
	~GLFWController(); 

	CONTROLLER_BUTTON button;
	CONTROLLER_STICKS stick;

	bool isConnected();
	float GetJoyStickButtonPressed(CONTROLLER_BUTTON buttonQuery);
	float GetJoyStickTriggerPressed(CONTROLLER_STICKS stickQuery);
	bool joystick_callback(int joy, int Event);
	
};
#endif