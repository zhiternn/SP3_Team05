/******************************************************************************/
/*!
\file	Projectile.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to create Projectiles
*/
/******************************************************************************/
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"
#include "Vector3.h"

/******************************************************************************/
/*!
Class GameObject.h:
\brief	Creates Projectiles for the game
*/
/******************************************************************************/
class CProjectile : public GameObject
{
public:
	enum PROJECTILE_TYPE
	{
		BULLET,
		SHIELD,
		HOOK,
		TRAP,

		MAX,
	};

	CProjectile(PROJECTILE_TYPE type = PROJECTILE_TYPE::BULLET);    //< Default Constructor
	virtual ~CProjectile();													//< Destructor

	void Init(Vector3 pos, Vector3 dir);
	virtual void Update(double dt) = 0;									//< Abstract Update
	virtual void HandleInteraction(GameObject* b, double dt) = 0;		//< Handling Interactions
	virtual void Boost(float multiplier) = 0;
	virtual void HandleOutOfBounds(float minX, float maxX, float minY, float maxY);

	virtual void UpgradeEffect(float amount) = 0;

	float GetDMG();													//< Returns Projectile Damage
	virtual float GetEffect();
	float GetLifetime();											//< Returns Projectile Lifetime								//< Returns Projectile Type
	PROJECTILE_TYPE GetProjType();
	float GetProjectileSpeed();
	
	//< Setters
	void SetDMG(float damage);										//< Set Projectile Damage
	void SetLifetime(float lifetime);								//< Set Projectile Lifetime
	void SetProjectileSpeed(float speed);
	void SetProjType(PROJECTILE_TYPE type);

	PROJECTILE_TYPE proj_type;

protected:
	//< Variables
	float proj_dmg;													//< Projectile Damage
	float proj_lifetime;											//< Projectile Lifetime
	float proj_speed;

};

#endif /* PROJECTILE_H */