#include "WeaponList.h"

void Shotgun::Fire(Vector3 pos, Vector3 dir, CProjectile::PROJECTILE_TEAM team)
{
	//if (fireRate <= 0.0f)
	{
		for (int i = 0; i < projectileCount; ++i)
		{
			Vector3 offset = Vector3(
				Math::RandFloatMinMax(-spread, spread),
				Math::RandFloatMinMax(-spread, spread),
				0
				);

			CProjectile* proj = FetchProjectile();
			*proj = *projectileInfo;
			proj->Init(pos, dir + offset);
			proj->SetTeam(team);
		}
	}
}

void MachineGun::Fire(Vector3 pos, Vector3 dir, CProjectile::PROJECTILE_TEAM team)
{
	//if (fireRate <= 0.0f)
	{
		CProjectile* proj = FetchProjectile();
		*proj = *projectileInfo;
		proj->Init(pos, dir);
		proj->SetTeam(team);
	}
}

void Trap::Fire(Vector3 pos, Vector3 dir, CProjectile::PROJECTILE_TEAM team)
{
	CProjectile *proj = FetchProjectile();
	*proj = *projectileInfo;
	proj->SetTeam(team);
	proj->SetVelocity(0, 0, 0);
	proj->Init(pos, dir - dir);
}