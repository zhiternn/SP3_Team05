#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

class Entity : public GameObject
{
public:
	enum ENTITY_TYPE
	{
		ENTITY_PLAYER,
		ENTITY_BOSS_MAIN,
		ENTITY_BOSS_BODY,

		ENTITY_END,
	};
	Entity(ENTITY_TYPE type = ENTITY_END);
	~Entity();

	virtual void TakeDamage(unsigned amount);
	virtual void Die();

	bool IsDead();
	unsigned int GetHP();
	ENTITY_TYPE GetEntityType();

protected:
	ENTITY_TYPE entityType;
	short int health;
	bool isDead;

};

#endif