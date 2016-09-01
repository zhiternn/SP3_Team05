#ifndef CONTROLS_H
#define CONTROLS_H

#include "Button.h"

class Controls
{
public:
	static Controls& GetInstance()
	{
		static Controls app;
		return app;
	}

	enum KEY_INPUTS
	{
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_0,
		KEY_NUMPAD_0,
		KEY_NUMPAD_1,
		KEY_NUMPAD_2,
		KEY_NUMPAD_3,
		KEY_NUMPAD_4,
		KEY_NUMPAD_5,
		KEY_NUMPAD_6,
		KEY_NUMPAD_7,
		KEY_NUMPAD_8,
		KEY_NUMPAD_9,

		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,

		KEY_ESC,
		KEY_SPACE,
		KEY_RETURN,
		KEY_LSHIFT,
		KEY_RSHIFT,
		KEY_LCTRL,
		KEY_RCTRL,
		KEY_BACKSPACE,

		MOUSE_LBUTTON,
		MOUSE_RBUTTON,

		KEY_END
	};
	
	float mouse_xOffset, mouse_yOffset;//returns how much the mouse moved between frames (double)
	short int  mouse_ScrollX, mouse_ScrollY;//returns either 1 or -1 (scroll up or down)

	void UpdateInputs();
	bool OnPress(Controls::KEY_INPUTS key);
	bool OnHold(Controls::KEY_INPUTS key);
	bool OnRelease(Controls::KEY_INPUTS key);

private:
	Controls();
	~Controls();

	Button buttons[KEY_END];
};

#endif