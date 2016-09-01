#ifndef SUMMONS_H
#define SUMMONS_H

#include "Enemy.h"
#include "WeaponList.h"

static const float SUMMON_TOUCH_DAMAGE = 5.f;

class Summons : public Enemy
{
public:
	Summons();
	~Summons();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject* b, double dt);
	virtual void SetupMesh();
	virtual void Die();
	void Goto(Vector3 pos);
	void Shoot(Vector3 dir);
	bool isDefending;

	Weapon *weapon;

private:
	short randWeapon;
	short randProjectile;


};


#endif // !SUMMONS_H
