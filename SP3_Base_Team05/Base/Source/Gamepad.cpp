#include "Gamepad.h"
#include "MyMath.h"

//Link 'XInput' Library
#pragma comment(lib, "Xinput.lib")

XButtonIDs XButtons;

XButtonIDs::XButtonIDs()
{
	A = 0;
	B = 1;
	X = 2;
	Y = 3;

	DPad_Up = 4;
	DPad_Down = 5;
	DPad_Left = 6;
	DPad_Right = 7;

	L_Shoulder = 8;
	R_Shoulder = 9;

	L_Thumbstick = 10;
	R_Thumbstick = 11;

	Start = 12;
	Back = 13;
}
//Default Constructor and Destructors
Gamepad::Gamepad()
{

}

Gamepad::Gamepad(int index)
{
	//Set Gamepad Index
	m_GamepadIndex = index - 1;

	//Iterate through all gamepad buttons
	for (int i = 0; i < ButtonCount; i++)
	{
		Prev_ButtonStates[i] = false;
		ButtonStates[i] = false;
		Gamepad_ButtonsDown[i] = false;
	}
}

Gamepad::~Gamepad()
{

}

//= Getters
XINPUT_STATE Gamepad::GetState()
{
	//temp state to return
	XINPUT_STATE CurrentState;

	//Zero Memory
	ZeroMemory(&CurrentState, sizeof(XINPUT_STATE));

	//Get Current Gamepad State
	XInputGetState(m_GamepadIndex, &CurrentState);

	return CurrentState;
}

int Gamepad::GetIndex()
{
	return m_GamepadIndex;
}

bool Gamepad::IsConnected()
{
	//Zero Memory
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));

	//Get Current Gamepad State
	DWORD Result = XInputGetState(m_GamepadIndex, &m_State);

	if (Result == ERROR_SUCCESS)
	{
		//Gamepad is Connected
		return true;
	}
	else
	{
		//Gamepad is not connected
		return false;
	}
}

//= Update Gamepad State
void Gamepad::Update()
{
	//Obtain Current Gamepad State
	m_State = GetState(); 

	//Iterate through gamepad buttons
	for (int i = 0; i < ButtonCount; i++)
	{
		//Set Button State for current frame
		ButtonStates[i] = m_State.Gamepad.wButtons & XINPUT_Buttons[i];
		Gamepad_ButtonsDown[i] = !Prev_ButtonStates[i] && ButtonStates[i];
	}
}

void Gamepad::RefreshState()
{
	memcpy(Prev_ButtonStates, ButtonStates, sizeof(Prev_ButtonStates));
}
//= Thumbstick Deadzone checks
bool Gamepad::L_Stick_InDeadzone()
{
	//obtain X & Y axes of the stick
	short s_X = m_State.Gamepad.sThumbLX;
	short s_Y = m_State.Gamepad.sThumbLY;

	//X axis is outside of deadzone
	if (s_X > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || s_X < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return false;
	}

	// Y Axis outside of deadzone
	if (s_Y > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || s_Y < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return false;
	}

	//Either one or both axes are within deadzone
	return true;
}

bool Gamepad::R_Stick_InDeadzone()
{
	//obtain X & Y axes of the stick
	short s_X = m_State.Gamepad.sThumbRX;
	short s_Y = m_State.Gamepad.sThumbRY;

	//X axis is outside of deadzone
	if (s_X > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || s_X < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return false;
	}

	// Y Axis outside of deadzone
	if (s_Y > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || s_Y < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return false;
	}

	//Either one or both axes are within deadzone
	return true;
}

//Return Axis of Thumbstick

//= Returns X axis of Left Stick
float Gamepad::Left_Stick_X()
{
	short s_X = m_State.Gamepad.sThumbLX;

	return (static_cast<float>(s_X) / 32768.0f);
}

//= Returns Y axis of Left Stick
float Gamepad::Left_Stick_Y()
{
	short s_Y = m_State.Gamepad.sThumbLY;

	return (static_cast<float>(s_Y) / 32768.0f);
}

//= Returns X axis of Right Stick
float Gamepad::Right_Stick_X()
{
	short s_X = m_State.Gamepad.sThumbRX;

	return (static_cast<float>(s_X) / 32768.0f);
}

//= Returns Y axis of Right Stick
float Gamepad::Right_Stick_Y()
{
	short s_Y = m_State.Gamepad.sThumbRY;

	return (static_cast<float>(s_Y) / 32768.0f);
}

//Returns value of SJWs

//= Returns Left Trigger Value
float Gamepad::LeftTrigger()
{
	//Obtain value of left trigger
	BYTE Trigger = m_State.Gamepad.bLeftTrigger;

	//Obtain (float) value of right trigger
	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		//Trigger is pressed
		return (Trigger / 255.0f);
	}
	//Trigger is not pressed
	else
		return 0.0f;
}


//= Returns Right Trigger Value
float Gamepad::RightTrigger()
{
	BYTE Trigger = m_State.Gamepad.bRightTrigger;

	//Obtain (float) value of right trigger
	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		//Trigger is pressed
		return (Trigger / 255.0f);
	}
	//Trigger is not pressed
	else
		return 0.0f;
}

/*
====<< EXAMPLES >>====
if (LeftTrigger() > 0.0f)
{
	// The trigger has been pressed (even slightest tap)...
}
*/

//Controller Vibration
void Gamepad::Rumble(float LeftMotor, float RightMotor)
{	
	Math::Clamp(LeftMotor, 0.f, 1.f);
	Math::Clamp(RightMotor, 0.f, 1.f);
	//Vibration state
	XINPUT_VIBRATION VibeState;

	//Zero Memory
	ZeroMemory(&VibeState, sizeof(XINPUT_VIBRATION));

	//Calculate Vibration Values
	int iLeftMotor = int(LeftMotor * 65535.0f);
	int iRightMotor = int(RightMotor * 65535.0f);

	//Set Vibration Values
	VibeState.wLeftMotorSpeed = iLeftMotor;
	VibeState.wRightMotorSpeed = iRightMotor;

	//Set Vibration State
	XInputSetState(m_GamepadIndex, &VibeState);
}

/*
====<< EXAMPLES >>====

// Only rumble left
Rumble(1.0f, 0.0f);

// Only rumble right
Rumble(0.0f, 0.6f);

// Rumble both
Rumble(0.3f, 0.3f);

// Cancel rumble
Rumble();
*/

//= Returns True if Button is Pressed [NON FRAME SPECIFIC]
bool Gamepad::GetButtonPressed(int Button)
{
	if (m_State.Gamepad.wButtons & XINPUT_Buttons[Button])
	{
		//Button is pressed
		return true;
	}
	else
		//No press
		return false;
}

//= Returns True if Button is Pressed [FRAME SPECIFIC]
bool Gamepad::GetButtonDown(int Button)
{
	return Gamepad_ButtonsDown[Button];
}
