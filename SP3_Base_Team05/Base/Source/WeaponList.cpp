#include "WeaponList.h"

void Shotgun::Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team)
{
	if (shootDelay <= 0.0f)
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

void MachineGun::Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team)
{
	std::cout << shootDelay << std::endl;
	if (shootDelay <= 0.0f)
	{
		CProjectile* proj = FetchProjectile();
		*proj = *projectileInfo;
		proj->Init(pos, dir);
		proj->SetTeam(team);
		
	}
}

//< DETLAFF WEAPONS
// SET AS RANDOM FOR NOW
void DetlaffArc::Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team)
{
	if (shootDelay <= 0.0f)
	{
		for (int i = 0; i < projectileCount; i++)
		{
			float split = spread / projectileCount;
			float hehe_xd = split / 2;



			float offsetVal = -(spread) + (i * split);

			Vector3 offset = Vector3(
				Math::RandFloatMinMax(-spread, spread),
				-Math::RandFloatMinMax(-spread, spread),
				0
				);

	
			std::cout << offsetVal << std::endl;


			CProjectile* proj = FetchProjectile();
			*proj = *projectileInfo;
			proj->Init(pos, dir + offset);
			proj->SetTeam(team);
		}

	}
}