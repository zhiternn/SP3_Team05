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

class Hook : public CProjectile
{
public:
	Hook() :CProjectile(PROJECTILE_TYPE::HOOK){
		SetDMG(0);
		SetLifetime(2);
		SetProjectileSpeed(50.0f);
		SetScale(1, 1, 1);
		SetMass(1);
	}
	~Hook(){}

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

	//void CalculateChance(Enemy *enemy);

	bool Capture();
private:

	float captureChance;
};

class Shield : public CProjectile
{
public:
    Shield() : CProjectile(PROJECTILE_TYPE::SHIELD) {
        CurrHealth = 2000;
        MaxHealth = 2000;
        proj_dmg = 0;
        aliveState = true;
        startShieldRegen = false;
        proj_speed = 0;
        proj_lifetime = 10;
    }//< Overloaded Constructor
    ~Shield(){}                                                          //< Destructor

    virtual void Init(Vector3 pos);                                     //< Initialize
    virtual void Update(double dt);                                     //< Update
    virtual void HandleInteraction(GameObject* b, double dt);           //< Handles Interaction for Shotgun Shell
    virtual void TakeDamage(unsigned amount);

    void regenerateShield(float currHP, double dt);                     //< Regenerates Shield Health

    // Getter
    //bool GetActiveState();                                              //< Sets Shield Active State
    bool GetAliveState();                                               //< Sets Shield Alive State
    float GetCurrHealth();                                              //< Sets Shield Curr Health
    float GetMaxHealth();                                               //< Sets Shield Max Health

    // Setter
    //void SetActiveState(bool active);                                   //< Returns Shield Active State 
    void SetAliveState(bool alive);                                     //< Returns Shield Alive State
    void SetCurrHealth(float curr);                                     //< Returns Shield Curr Health
    void SetMaxHealth(float max);                                       //< Returns Shield Max Health

private:
    //bool activeState;
    bool aliveState;
    float CurrHealth;
    float MaxHealth;
    float startShieldRegen;
};

class Bullet : public CProjectile
{
public:
	Bullet():CProjectile(PROJECTILE_TYPE::BULLET){
		SetDMG(10);
		SetLifetime(2);
		SetProjectileSpeed(150.f);
		SetScale(.5f, .5f, .5f);
	}					//< Overloaded Constructor
	~Bullet(){};																					//< Destructor

	virtual void Update(double dt);																//< Update
	virtual void HandleInteraction(GameObject* b, double dt);									//< Handles Interaction for Bullet
};

Bullet* FetchBullet();
Hook* FetchHook();

#endif // !PROJECTILELIST_H
