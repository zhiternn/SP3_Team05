#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun():projectileCount(5), spread(0.25f){
		SetFireRate(1.5);
	}
	~Shotgun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:
	float spread;
	int projectileCount;

};

class MachineGun : public Weapon
{
public:
	MachineGun(){
		SetFireRate(5);
	}
	~MachineGun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:

};

//< Wayne's Boss
//< DETLAFF - BOSS WEAPONS

class SplitGun : public Weapon
{
public:
	SplitGun() : coneAngle(45.0f), projectileCount(2) {
		SetFireRate(1);
	}
	~SplitGun(){}

	virtual void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:
	float coneAngle;
	int projectileCount;

};

#endif // !WEAPONLIST_H
