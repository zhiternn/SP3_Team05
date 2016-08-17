#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include "Projectile.h"

class Rope : public CProjectile
{
public:
	Rope():CProjectile(PROJECTILE_TYPE::ROPE){}
	~Rope(){}

	virtual void HandleInteraction(GameObject* b, double dt);

private:

};

class Shield : public CProjectile
{
public:
	Shield():CProjectile(PROJECTILE_TYPE::SHIELD){}
	~Shield(){}

	virtual void HandleInteraction(GameObject* b, double dt);

private:

};


class Trap : public CProjectile
{
	Trap() :CProjectile(PROJECTILE_TYPE::TRAP){}
	~Trap(){}

	virtual void HandleInteraction(GameObject *b, double dt);

	void CalculateChance(unsigned int enemyHealth);

	bool Capture();
private:

	float captureChance;
};
#endif // !PROJECTILELIST_H
