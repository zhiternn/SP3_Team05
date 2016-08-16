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
	virtual void Fire(Vector3 pos, Vector3 dir) = 0;
	void AssignProjectile(CProjectile* proj);

protected:
	Weapon();
	
	float fireRate;
	float projectileSpeed;
	CProjectile* projectileInfo;
};


#endif /* WEAPON_H */
