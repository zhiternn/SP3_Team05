#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

class Entity : public GameObject
{
public:
	Entity();
	~Entity();

	virtual void TakeDamage(unsigned amount);
	bool IsDead();
	unsigned int GetHP();

protected:
	short int health;
	bool isDead;

};

#endif