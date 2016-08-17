#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun():projectileCount(5), spread(0.8f){}
	~Shotgun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:
	float spread;
	int projectileCount;

};

class MachineGun : public Weapon
{
public:
	MachineGun(){}
	~MachineGun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:

};

class Trap : public Weapon
{
public:
	Trap(){}
	~Trap(){}

	virtual void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:
	float m_timer;
};

#endif // !WEAPONLIST_H
