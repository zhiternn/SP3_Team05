#include "Weapon.h"

Weapon::Weapon():
projectileInfo(NULL),
fireRate(1.0f),
shootDelay(0)
{
}

Weapon::~Weapon()
{
	if (projectileInfo)
		delete projectileInfo;
}

void Weapon::Update(double dt)
{
	if (shootDelay >= 0)
	{
		shootDelay -= fireRate * dt;
	}
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
