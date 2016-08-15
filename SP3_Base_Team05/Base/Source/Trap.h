#pragma once
#include "Weapon.h"
class Trap : public Weapon
{
private:
	Vector3 pos;
public:
	Trap();
	~Trap();
	
	Vector3 getPosition();
	void SpawnTrap(Vector3 pos);

};

