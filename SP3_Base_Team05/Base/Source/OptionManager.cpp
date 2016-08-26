#include "OptionManager.h"

OptionManager::OptionManager()
{

}
OptionManager::~OptionManager()
{

}

bool OptionManager::UseControl()
{
	return Control;
}
void OptionManager::SetControl(bool controller)
{
	this->Control = controller;  
}