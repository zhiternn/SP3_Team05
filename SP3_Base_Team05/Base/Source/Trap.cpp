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
	GameObject *m_trap = new GameObject(GameObject::GO_CUBE);
	m_trap->SetScale(0.5, 0.5, 0.5);
	//Spawn the GO at [pos] coordinates
}