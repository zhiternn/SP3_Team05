#include "Controls.h"
#include <windows.h>

Controls::Controls()
{
	buttons[KEY_1].AssignHotKey('1');
	buttons[KEY_2].AssignHotKey('2');
	buttons[KEY_3].AssignHotKey('3');
	buttons[KEY_4].AssignHotKey('4');
	buttons[KEY_5].AssignHotKey('5');
	buttons[KEY_6].AssignHotKey('6');
	buttons[KEY_7].AssignHotKey('7');
	buttons[KEY_8].AssignHotKey('8');
	buttons[KEY_9].AssignHotKey('9');
	buttons[KEY_0].AssignHotKey('0');
	buttons[KEY_NUMPAD_0].AssignHotKey(VK_NUMPAD0);
	buttons[KEY_NUMPAD_1].AssignHotKey(VK_NUMPAD1);
	buttons[KEY_NUMPAD_2].AssignHotKey(VK_NUMPAD2);
	buttons[KEY_NUMPAD_3].AssignHotKey(VK_NUMPAD3);
	buttons[KEY_NUMPAD_4].AssignHotKey(VK_NUMPAD4);
	buttons[KEY_NUMPAD_5].AssignHotKey(VK_NUMPAD5);
	buttons[KEY_NUMPAD_6].AssignHotKey(VK_NUMPAD6);
	buttons[KEY_NUMPAD_7].AssignHotKey(VK_NUMPAD7);
	buttons[KEY_NUMPAD_8].AssignHotKey(VK_NUMPAD8);
	buttons[KEY_NUMPAD_9].AssignHotKey(VK_NUMPAD9);

	buttons[KEY_A].AssignHotKey('A');
	buttons[KEY_B].AssignHotKey('B');
	buttons[KEY_C].AssignHotKey('C');
	buttons[KEY_D].AssignHotKey('D');
	buttons[KEY_E].AssignHotKey('E');
	buttons[KEY_F].AssignHotKey('F');
	buttons[KEY_G].AssignHotKey('G');
	buttons[KEY_H].AssignHotKey('H');
	buttons[KEY_I].AssignHotKey('I');
	buttons[KEY_J].AssignHotKey('J');
	buttons[KEY_K].AssignHotKey('K');
	buttons[KEY_L].AssignHotKey('L');
	buttons[KEY_M].AssignHotKey('M');
	buttons[KEY_N].AssignHotKey('N');
	buttons[KEY_O].AssignHotKey('O');
	buttons[KEY_P].AssignHotKey('P');
	buttons[KEY_Q].AssignHotKey('Q');
	buttons[KEY_R].AssignHotKey('R');
	buttons[KEY_S].AssignHotKey('S');
	buttons[KEY_T].AssignHotKey('T');
	buttons[KEY_U].AssignHotKey('U');
	buttons[KEY_V].AssignHotKey('V');
	buttons[KEY_W].AssignHotKey('W');
	buttons[KEY_X].AssignHotKey('X');
	buttons[KEY_Y].AssignHotKey('Y');
	buttons[KEY_Z].AssignHotKey('Z');

	buttons[KEY_ESC].AssignHotKey(VK_ESCAPE);
	buttons[KEY_SPACE].AssignHotKey(VK_SPACE);
	buttons[KEY_RETURN].AssignHotKey(VK_RETURN);
	buttons[KEY_LSHIFT].AssignHotKey(VK_LSHIFT);
	buttons[KEY_RSHIFT].AssignHotKey(VK_RSHIFT);
	buttons[KEY_LCTRL].AssignHotKey(VK_LCONTROL);
	buttons[KEY_RCTRL].AssignHotKey(VK_RCONTROL);
	buttons[KEY_BACKSPACE].AssignHotKey(VK_BACK);

	buttons[MOUSE_LBUTTON].AssignHotKey(VK_LBUTTON);
	buttons[MOUSE_RBUTTON].AssignHotKey(VK_RBUTTON);
}

Controls::~Controls()
{
}

void Controls::UpdateInputs()
{
	for (int i = 0; i < KEY_END; ++i)
	{
		buttons[i].Update();
	}
}

bool Controls::OnPress(Controls::KEY_INPUTS key)
{
	return buttons[key].onPress;
}

bool Controls::OnHold(Controls::KEY_INPUTS key)
{
	return buttons[key].buttonState && !buttons[key].onPress;
}
bool Controls::OnRelease(Controls::KEY_INPUTS key)
{
	return buttons[key].onRelease;
}