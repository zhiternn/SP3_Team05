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


	//Spawn the GameObject at [pos] coordinates

	m_trap->SetActive(true);
	m_trap->SetScale(20, 20, 20);
	m_trap->SetFront(1, 0, 0);
	m_trap->SetPostion(pos);
	m_trap->SetType(GameObject::GO_BALL);
} 

void Trap::DestroyTrap()
{

}