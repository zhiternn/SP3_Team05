#include "Inventory.h"

Inventory::Inventory()
{
	weapons.push_back(new Splitgun());
	weapons.push_back(new MachineGun());
	weapons.push_back(new Shotgun());

	bullets.push_back(new Hook());
	bullets.push_back(new Bullet());
	bullets.push_back(new TrapProjectile());
}

Inventory::~Inventory()
{
}