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
		ROPE,
		TRAP,
		RAY,

		MAX,
	};

	CProjectile(PROJECTILE_TYPE type = PROJECTILE_TYPE::BULLET);    //< Default Constructor
	~CProjectile();													//< Destructor

	void Init(Vector3 pos, Vector3 dir);
	virtual void Update(double dt);									//< Abstract Update
	virtual void HandleInteraction(GameObject* b, double dt);		//< Handling Interactions

	float GetDMG();													//< Returns Projectile Damage
	float GetLifetime();											//< Returns Projectile Lifetime								//< Returns Projectile Type
	PROJECTILE_TYPE GetType();
	float GetProjectileSpeed();
	
	//< Setters
	void SetDMG(float damage);										//< Set Projectile Damage
	void SetLifetime(float lifetime);								//< Set Projectile Lifetime
	void SetProjectileSpeed(float speed);
	void SetType(PROJECTILE_TYPE type);

protected:
	//< Variables
	float proj_dmg;													//< Projectile Damage
	float proj_lifetime;											//< Projectile Lifetime
	float proj_speed;								
	PROJECTILE_TYPE proj_type;										//< Projectile Type
};

CProjectile* FetchProjectile();

#endif /* PROJECTILE_H */