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

	// Setters
	void SetHP(short int HP);

	// Getters
	short int GetHP();
	short int GetMaxHP();

	// Issers
	bool IsDead();

	ENTITY_TYPE GetEntityType();

	void SetEntityType(ENTITY_TYPE type);

protected:
	ENTITY_TYPE entityType;
	short int health;
	short int maxHealth;
	bool isDead;

};

#endif