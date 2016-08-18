#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun():projectileCount(36), spread(4.f){
		SetFireRate(0.5);
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

class DetlaffArc : public Weapon
{
public:
	DetlaffArc() : spread(4.f) , projectileCount(36) {
		SetFireRate(1);
	}
	~DetlaffArc(){}

	virtual void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:
	float spread;
	int projectileCount;

};

#endif // !WEAPONLIST_H
