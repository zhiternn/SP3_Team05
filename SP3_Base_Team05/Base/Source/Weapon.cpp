#include "Weapon.h"

Weapon::Weapon():
projectileInfo(NULL),
fireRate(1.0f),
shootDelay(0.1f)
{
}

Weapon::~Weapon()
{
	//if (projectileInfo)
	//	delete projectileInfo;
}

void Weapon::Update(double dt)
{
	if (shootDelay > 0.0f)
	{
		shootDelay -= fireRate * dt;
	}
}

void Weapon::AssignProjectile(CProjectile* proj)
{
	projectileInfo = proj;
}

float Weapon::GetFireRate()
{
	return this->fireRate;
}

void Weapon::SetFireRate(float fireRate)
{
	this->fireRate = fireRate;
}

CProjectile* Weapon::GetProjInfo()
{
	if (projectileInfo != NULL)
		return projectileInfo;
	else
		return NULL;
}