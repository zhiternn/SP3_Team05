#include "GLFWController.h"
#include <iostream>

GLFWController::GLFWController()
{
}

GLFWController::~GLFWController()
{

}


float GLFWController::GetJoyStickButtonPressed(CONTROLLER_BUTTON buttonQuery)
{
	int buttonArray;


	const unsigned char *Button = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &buttonArray);

	return Button[buttonQuery];
}

float GLFWController::GetJoyStickTriggerPressed(CONTROLLER_STICKS stickQuery)
{
	int stickArray;


	const float *Stick = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &stickArray);

	return Stick[stickQuery];
}


bool GLFWController::isConnected()
{
	int present = glfwJoystickPresent(GLFW_JOYSTICK_2);

	return present;
}