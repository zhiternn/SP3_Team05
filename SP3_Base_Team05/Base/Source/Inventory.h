#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

#include "WeaponList.h"
#include "ProjectileList.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	std::vector<Weapon*> weapons;
	std::vector<CProjectile*> bullets;

private:

};

#endif // !INVENTORY_H
