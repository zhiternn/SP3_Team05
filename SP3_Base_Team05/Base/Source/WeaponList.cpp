#include "WeaponList.h"
#include "Mtx44.h"

void Shotgun::Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team)
{
	if (shootDelay <= 0.0f)
	{
		shootDelay = 1.0f;
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

void MachineGun::Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team)
{
	if (shootDelay <= 0.0f)
	{
		shootDelay = 1.0f;
		CProjectile* proj = FetchProjectile();
		*proj = *projectileInfo;
		proj->Init(pos, dir);
		proj->SetTeam(team);
	}
}

//< DETLAFF WEAPONS
// SET AS RANDOM FOR NOW
void SplitGun::Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team)
{
	if (shootDelay <= 0.0f)
	{
		shootDelay = 1.0f;
		Mtx44 rotate;
		float offsetAngle;
		float initialAngle = -coneAngle / 2;

		if (projectileCount % 2 == 0)//even number of projectileCount
		{
			offsetAngle = coneAngle / (projectileCount);
			initialAngle += offsetAngle / 2;
		}
		else//odd number of projectileCount
		{
			offsetAngle = coneAngle / (projectileCount - 1);
		}

		for (int i = 0; i < projectileCount; ++i)
		{
			rotate.SetToRotation(initialAngle + offsetAngle * i, 0, 0, 1);

			CProjectile* proj = FetchProjectile();
			*proj = *projectileInfo;
			proj->SetLifetime(2.0f);
			proj->SetScale(Vector3(2.0f, 2.0f, 2.0f));
			proj->Init(pos, rotate * dir);
			proj->SetTeam(team);
		}
	}
}

void SplitGun::SetAngle(float angle)
{
	this->coneAngle = angle;
}
void SplitGun::SetCount(int count)
{
	this->projectileCount = count;
}

