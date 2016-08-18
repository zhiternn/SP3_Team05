#pragma once

#include "Enemy.h"
#include "Weapon.h"
class CDetlaff : public Enemy 
{
public:
	CDetlaff();
	~CDetlaff();

	virtual void Update(double dt);
	virtual void HandleInteraction(GameObject *b, double dt);

	void FireOnTarget(Entity *target);

	enum BOSS_STATE
	{
		STATE_1,
		STATE_2,
		STATE_3,
		STATE_END
	};
	BOSS_STATE state;
	//BOSS Cooldown before firing
private: 
	Entity *target;
	Weapon* weapon;
};

