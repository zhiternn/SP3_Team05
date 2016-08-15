#include "Trap.h"


Trap::Trap()
{
}


Trap::~Trap()
{
}

Vector3 Trap::getPosition()
{
	return Vector3(this->pos.x, this->pos.y);
}

void Trap::SpawnTrap(Vector3 pos)
{
	w_active = true;

	//Spawn the GO at [pos] coordinates
}