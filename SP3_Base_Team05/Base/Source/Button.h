#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:
	Button();
	~Button();

	void AssignHotKey(unsigned short hotkey);
	void Update();

	unsigned char hotkey;
	bool buttonState;
	bool onPress;
	bool onRelease;

private:

};



#endif