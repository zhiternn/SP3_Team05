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
		SetProjectileSpeed(100.f);
		SetScale(2, 2, 2);
		SetMass(1);
	}
	~Shield(){}

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);

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

	void CalculateChance(unsigned int enemyHealth);

	bool Capture();
private:

	float captureChance;
};
#endif // !PROJECTILELIST_H
