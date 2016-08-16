#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun():projectileCount(5), spread(0.2f){}
	~Shotgun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir);

private:
	float spread;
	int projectileCount;

};

class MachineGun : public Weapon
{
public:
	MachineGun(){}
	~MachineGun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir);

private:

};

#endif // !WEAPONLIST_H
