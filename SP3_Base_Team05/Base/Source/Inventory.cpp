#include "Inventory.h"

Inventory::Inventory()
{
	weapons.push_back(new MachineGun());
	weapons.push_back(new Shotgun());
	weapons.push_back(new Splitgun());

	bullets.push_back(new Bullet());
	bullets.push_back(new Hook());
}

Inventory::~Inventory()
{
}