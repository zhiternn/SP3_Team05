#include "Weapon.h"

Weapon::Weapon():
projectileInfo(NULL),
fireRate(1.0f)
{
}

Weapon::~Weapon()
{
	if (projectileInfo)
		delete projectileInfo;
}

void Weapon::AssignProjectile(CProjectile* proj)
{
	if (projectileInfo)
		delete projectileInfo;

	projectileInfo = new CProjectile();
	*projectileInfo = *proj;
}

float Weapon::GetFireRate()
{
	return this->fireRate;
}

void Weapon::SetFireRate(float fireRate)
{
	this->fireRate = fireRate;
}
