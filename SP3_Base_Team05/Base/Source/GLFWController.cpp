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

    if (this->isConnected())
    {
        const unsigned char *Button = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &buttonArray);

        return Button[buttonQuery];
    }
    return 0;
}

float GLFWController::GetJoyStickTriggerPressed(CONTROLLER_STICKS stickQuery)
{
	int stickArray;

    if (this->isConnected())
    {
        const float *Stick = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &stickArray);

        return Stick[stickQuery];
    }
    return 0;
}


bool GLFWController::isConnected()
{
	int present = glfwJoystickPresent(GLFW_JOYSTICK_2);

	return present;
}