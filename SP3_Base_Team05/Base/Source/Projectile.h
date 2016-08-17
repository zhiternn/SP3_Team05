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
	enum PROJECTILE_TEAM
	{
		TEAM_NEUTRAL = 0,

		TEAM_PLAYER,
		TEAM_ENEMY,

		TEAM_END
	};
	enum PROJECTILE_TYPE
	{
		BULLET,
		SHIELD,
		ROPE,
		TRAP,
		RAY,

		MAX,
	};

	CProjectile(PROJECTILE_TYPE type = PROJECTILE_TYPE::BULLET);                      //< Default Constructor
	~CProjectile();                                         //< Destructor

	void Init(Vector3 pos, Vector3 dir, float speed);
	virtual void Update(double dt);									//< Abstract Update
	virtual void HandleInteraction(GameObject* b, double dt);		//< Handling Interactions

	//< Getters
	float GetDMG();												//< Returns Projectile Damage
	float GetLifetime();										//< Returns Projectile Lifetime
	PROJECTILE_TEAM GetTeam();									//< Returns Projectile Type
	
	//< Setters
	void SetDMG(float damage);									//< Set Projectile Damage
	void SetLifetime(float lifetime);							//< Set Projectile Lifetime
	void SetTeam(PROJECTILE_TEAM team);							//< Set Projectile Type

protected:
	//< Variables
	float proj_dmg;													//< Projectile Damage
	float proj_lifetime;											//< Projectile Lifetime
	PROJECTILE_TEAM proj_team;										//< Projectile Type
	PROJECTILE_TYPE proj_type;

};

CProjectile* FetchProjectile();

#endif /* PROJECTILE_H */