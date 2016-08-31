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
	void AddCurrency(int currency);
	bool IsDeducting(int currency);

	// Setters
	void SetCurrency(int currency);

	// Getters
	int GetCurrency();

private:
	int currency;

};

#endif // !INVENTORY_H
