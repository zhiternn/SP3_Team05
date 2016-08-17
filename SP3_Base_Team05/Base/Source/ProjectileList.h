/******************************************************************************/
/*!
\file	ProjectileList.h
\author Lee Yu Xian and Quek Tse Hiang Reuben
\par	email: 155118A\@nyp.edu.sg and 152773F\@nyp.edu.sg
\brief
Pure Abstract Class to create Projectiles
*/
/******************************************************************************/

#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include "Projectile.h"
#include "Enemy.h"

/******************************************************************************/
/*!
Class ProjectileList.h:
\brief	Creates Projectiles for the game
*/
/******************************************************************************/
class ShotgunShell : public CProjectile
{
public:
    ShotgunShell() :CProjectile(PROJECTILE_TYPE::BULLET){
        SetDMG(0);
        SetLifetime(2);
        SetProjectileSpeed(30.0f);
        SetScale(1, 1, 1);
        SetMass(1);
    }
    ~ShotgunShell(){}

    virtual void Update(double dt);                                     //< Update Shotgun Shell Stuff 
    virtual void HandleInteraction(GameObject* b, double dt);           //< Handles Interaction for Shotgun Shell

private:

};

class Rope : public CProjectile
{
public:
	Rope():CProjectile(PROJECTILE_TYPE::ROPE){
		SetDMG(0);
		SetLifetime(2);
		SetProjectileSpeed(1.0f);
		SetScale(1, 1, 1);
		SetMass(1);
	}
	~Rope(){}

    virtual void Update(double dt);                                     //< Update Rope Stuff
	virtual void HandleInteraction(GameObject* b, double dt);           //< Handles Interaction for Rope

private:
	
};


class Trap : public CProjectile
{
public:
	Trap():CProjectile(PROJECTILE_TYPE::TRAP){
		SetDMG(10);
		SetLifetime(30);
		SetProjectileSpeed(0);
		SetScale(1, 1, 1);
	}
	~Trap(){}

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject *b, double dt);

	void CalculateChance(Enemy *enemy);

	bool Capture();
private:

	float captureChance;
};
#endif // !PROJECTILELIST_H
