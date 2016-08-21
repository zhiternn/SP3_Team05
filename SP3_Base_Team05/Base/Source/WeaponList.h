#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun():projectileCount(5), spread(0.25f){
		SetFireRate(5);
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
		SetFireRate(10);
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
	SplitGun() : coneAngle(45.0f), projectileCount(10) {
		SetFireRate(5);
		state = STATE_1;
	}
	//Overloaded SplitGun 
	SplitGun(float angle, int count)
	{
		this->SetAngle(angle);
		this->SetCount(count);
		SetFireRate(2);
		state = STATE_2;
	}
	~SplitGun(){}

	enum WEAPON_STATE
	{
		STATE_1,
		STATE_2,
		NUM_STATES
	};

	virtual void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);
	
	//For weapon switching 
	void SetAngle(float angle);
	void SetCount(int count);

private:
	float coneAngle;
	int projectileCount;
	WEAPON_STATE state;

};

#endif // !WEAPONLIST_H
