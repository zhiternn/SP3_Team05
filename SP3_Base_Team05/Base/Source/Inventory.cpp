#include "Inventory.h"

Inventory::Inventory() :
currency(0)
{
	weapons.push_back(new Shotgun());
	weapons.push_back(new Splitgun());
	weapons.push_back(new MachineGun());

	bullets.push_back(new Bullet());
	bullets.push_back(new Hook());
	bullets.push_back(new TrapProjectile());
}

Inventory::~Inventory()
{
}

void Inventory::AddCurrency(unsigned currency)
{
	this->currency += currency;
}

bool Inventory::IsDeducting(unsigned currency)
{
	if (this->currency - currency >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Inventory::SetCurrency(unsigned currency)
{
	this->currency = currency;
}

unsigned Inventory::GetCurrency()
{
	return currency;
}