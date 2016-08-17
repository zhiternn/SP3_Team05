#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

class Entity : public GameObject
{
public:
	Entity();
	~Entity();

	void TakeDamage(unsigned amount);
	bool IsDead();

protected:
	short int health;
	bool isDead;

};

#endif