#include "Detlaff.h"


CDetlaff::CDetlaff() : target(NULL)
{
}


CDetlaff::~CDetlaff()
{
	if (target)
		delete target;
}

void CDetlaff::Update(double dt)
{
	GameObject::Update(dt);
}
