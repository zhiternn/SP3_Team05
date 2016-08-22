#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun():projectileCount(5), spread(0.25f){
		SetFireRate(5);
	}
	Shotgun(int projCount, float spread, float fireRate)
	{
		this->projectileCount = projCount;
		this->spread = spread;
		this->SetFireRate(fireRate);
	}
	~Shotgun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);
	float spread;
private:
	int projectileCount;

};

class MachineGun : public Weapon
{
public:
	MachineGun(){
		SetFireRate(10);
	}
	MachineGun(float firerate){
		SetFireRate(firerate);
	}
	~MachineGun(){}

	virtual	void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);

private:

};

//< Wayne's Boss
//< DETLAFF - BOSS WEAPONS

class Splitgun : public Weapon
{
public:
	Splitgun() : coneAngle(60.0f), projectileCount(3) {
		SetFireRate(1);
	}
	Splitgun(float coneAngle, int projCount, float fireRate)
	{
		this->coneAngle = coneAngle;
		this->projectileCount = projCount;
		this->SetFireRate(1);
	}
	~Splitgun(){}

	virtual void Fire(Vector3 pos, Vector3 dir, GameObject::TEAM_TYPE team);
	
	//For weapon switching 
	void SetAngle(float angle);
	void SetCount(int count);

private:
	float coneAngle;
	int projectileCount;
};

#endif // !WEAPONLIST_H
