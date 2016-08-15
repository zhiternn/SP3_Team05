#include "Button.h"
#include <Windows.h>


bool IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Button::Button()
{
}

Button::~Button()
{
}

void Button::AssignHotKey(unsigned short hotkey)
{
	buttonState = false;
	onPress = false;
	onRelease = false;

	this->hotkey = hotkey;
}

void Button::Update()
{
	onPress = false;
	onRelease = false;

	if (!buttonState && IsKeyPressed(hotkey))
	{
		buttonState = true;
		onPress = true;
	}
	else if(buttonState && !IsKeyPressed(hotkey))
	{
		buttonState = false;
		onRelease = true;
	}
}