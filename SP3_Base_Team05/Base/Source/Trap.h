#pragma once
#include "Weapon.h"
#include "Vector3.h"
class Trap : public Weapon
{
private:
	Vector3 pos;
public:
	Trap();
	~Trap();
	
	Vector3 getPosition();
	void SpawnTrap(Vector3 pos);
	void DestroyTrap();

};

