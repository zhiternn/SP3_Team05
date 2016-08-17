/******************************************************************************/
/*!
\file	Weapon.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Generic Class for Weapons (Abstract Class)
*/
/******************************************************************************/
#ifndef WEAPON_H
#define WEAPON_H

#include "Vector3.h"
#include "Projectile.h"

/******************************************************************************/
/*!
Class Weapon.h:
\brief	Creates Weapon for the game.
*/
/******************************************************************************/
class Weapon
{
public:
	virtual ~Weapon();
	void Update(double dt);
	virtual void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team = CProjectile::TEAM_NEUTRAL) = 0;
	void AssignProjectile(CProjectile* proj);

	// Getters
	float GetFireRate();

	// Setters
	void SetFireRate(float fireRate);

protected:
	Weapon();
	
	float fireRate;
	float shootDelay;
	CProjectile* projectileInfo;
};


#endif /* WEAPON_H */
