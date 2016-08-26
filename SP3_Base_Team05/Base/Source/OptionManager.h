#pragma once


class OptionManager
{
private:
	bool Control;
public:
	OptionManager();
	~OptionManager();

	bool UseControl();
	void SetControl(bool controller);

	static OptionManager& GetInstance()
	{
		static OptionManager om;
		return om;
	}


};