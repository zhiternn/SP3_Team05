#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include "Projectile.h"

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

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

private:
	

};

class Shield : public CProjectile
{
public:
	Shield() :CProjectile(PROJECTILE_TYPE::SHIELD){
		SetDMG(0);
		SetLifetime(2);
		SetProjectileSpeed(1.0f);
		SetScale(2, 2, 2);
		SetMass(1);
	}
	~Shield(){}

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

private:

};

#endif // !PROJECTILELIST_H
