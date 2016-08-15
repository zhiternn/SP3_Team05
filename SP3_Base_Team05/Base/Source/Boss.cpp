#include "Boss.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::Goto(Vector3 pos)
{
	if (destination.size() > 0)
	{
		Vector3 toDestination = (destination.top() - pos).Normalized(); //vel
	}
}

void Boss::AddDestination(Vector3 pos)
{
	destination.push(pos);
}