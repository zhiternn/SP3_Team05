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

void Inventory::AddCurrency(int currency)
{
	this->currency += currency;
}

bool Inventory::IsDeducting(int currency)
{
	if (this->currency - currency >= 0)
	{
		this->currency -= currency;
		return true;
	}
	else
	{
		return false;
	}
}

void Inventory::SetCurrency(int currency)
{
	this->currency = currency;
}

int Inventory::GetCurrency()
{
	return currency;
}