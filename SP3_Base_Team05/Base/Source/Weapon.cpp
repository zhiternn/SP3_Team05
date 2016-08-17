#include "Weapon.h"

Weapon::Weapon():
projectileInfo(NULL),
fireRate(1.0f),
projectileSpeed(50.0f)
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

float Weapon::GetProjectileSpeed()
{
	return this->projectileSpeed;
}

void Weapon::SetFireRate(float fireRate)
{
	this->fireRate = fireRate;
}

void Weapon::SetProjectileSpeed(float projectileSpeed)
{
	this->projectileSpeed = projectileSpeed;
}
