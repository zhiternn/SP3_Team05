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
	void AddCurrency(unsigned currency);
	bool IsDeducting(unsigned currency);

	// Setters
	void SetCurrency(unsigned currency);

	// Getters
	unsigned GetCurrency();

private:
	unsigned currency;

};

#endif // !INVENTORY_H
