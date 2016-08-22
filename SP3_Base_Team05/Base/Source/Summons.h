#ifndef SUMMONS_H
#define SUMMONS_H

#include "Enemy.h"
#include "WeaponList.h"

class Summons : public Enemy
{
public:
	Summons();
	~Summons();

	virtual void Init(Vector3 pos);
	virtual void Update(double dt);
	void Goto(Vector3 pos);
	void Shoot(Vector3 dir);

	bool isDefending;

	Weapon* weapon;

private:

};


#endif // !SUMMONS_H