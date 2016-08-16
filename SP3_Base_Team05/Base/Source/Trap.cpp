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
	Weapon *m_trap = new Weapon();

	m_trap->w_type = TRAP;


	//Spawn the GO at [pos] coordinates
}

void Trap::DestroyTrap()
{

}