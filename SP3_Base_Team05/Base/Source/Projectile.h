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
#include "Collider.h"

enum PROJECTILE_TYPE
{
    P_NEUTRAL = 0,

	P_PLAYER_BULLET,
	P_PLAYER_GRENADE,
	P_PLAYER_ROPE,
    P_ENEMY_BULLET,

    P_TOTAL
};

/******************************************************************************/
/*!
Class GameObject.h:
\brief	Creates Projectiles for the game
*/
/******************************************************************************/
class CProjectile : public GameObject
{
public:
	CProjectile();                                          //< Default Constructor
	~CProjectile();                                         //< Destructor

    void HandleInteraction(CProjectile* b, double dt);      //< Handling Interactions
	virtual void Update(double dt);                         //< Abstract Update

	//< Getters
    bool GetState();                                        //< Returns Projectile State
	float GetDMG();                                         //< Returns Projectile Damage
	float GetLifetime();                                    //< Returns Projectile Lifetime
    PROJECTILE_TYPE GetType();                              //< Returns Projectile Type
	
	//< Setters
	void SetState(bool state);                              //< Set Projectile State
	void SetDMG(float damage);                              //< Set Projectile Damage
	void SetLifetime(float lifetime);                       //< Set Projectile Lifetime
    void SetType(PROJECTILE_TYPE type);                     //< Set Projectile Type

private:
	//< Variables
	bool proj_active;                                       //< Projectile State
	float proj_dmg;                                         //< Projectile Damage
	float proj_lifetime;                                    //< Projectile Lifetime
    PROJECTILE_TYPE proj_type;                              //< Projectile Type
};

#endif /* PROJECTILE_H */