#ifndef GAMEPAD_H
#define GAMEPAD_H

//Controller input class

#include <Windows.h>
#include <Xinput.h>

//XInput Button Values
static const WORD XINPUT_Buttons[] =
{
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

//XInput Button IDs
struct XButtonIDs
{
	//default constructor
	XButtonIDs();
	
	//Main Action Buttons
	int A, B, X, Y; 

	//D-Pad
	int DPad_Up, DPad_Down, DPad_Left, DPad_Right;

	//Bumper Buttons

	int L_Shoulder, R_Shoulder;

	//Thumbsticks
	int L_Thumbstick, R_Thumbstick;

	//Start and Back Buttons
	int Start;
	int Back;
};

class Gamepad
{
public:
	//Constuctors and Destructors
	Gamepad();
	Gamepad(int index);
	~Gamepad();

	//Getters
	XINPUT_STATE GetState();
	int GetIndex();
	bool IsConnected();

	//Update Gamepad State
	void Update();
	void RefreshState();

	//Thumbstick Functions
	// = Return "true" if stick is inside deadzone
	// = Return "false" if stick is outside deadzone.
	bool L_Stick_InDeadzone();
	bool R_Stick_InDeadzone();

	float Left_Stick_X(); //Returns X axis of left stick
	float Left_Stick_Y(); //Returns Y axis of left stick
	float Right_Stick_X(); //Returns X axis of right stick
	float Right_Stick_Y(); //Returns Y axis of right stick

	//Trigger Functions 
	float LeftTrigger(); //Return Value of left trigger
	float RightTrigger(); //Return Value of right trigger

	//Button Functions
	bool GetButtonPressed(int Button);
	bool GetButtonDown(int Button);

	//Controller Vibration
	void Rumble(float LeftMotor = 0.f, float RightMotor = 0.f);

	
private:
	XINPUT_STATE m_State;
	//XINPUT_BATTERY_INFORMATION m_Battery;
	int m_GamepadIndex;

	static const int ButtonCount = 14;
	bool Prev_ButtonStates[ButtonCount];
	bool ButtonStates[ButtonCount];

	//Buttons pressed on current frame
	bool Gamepad_ButtonsDown[ButtonCount];
};

#endif //GAMEPAD_H

extern XButtonIDs XButtons;