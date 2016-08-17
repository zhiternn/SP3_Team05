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