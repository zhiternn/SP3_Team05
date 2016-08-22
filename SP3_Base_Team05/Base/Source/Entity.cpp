#include "Entity.h"

Entity::Entity():
GameObject(GameObject::GO_ENTITY),
health(100)
{
}

Entity::~Entity()
{
}

void Entity::TakeDamage(unsigned amount)
{
	health -= amount;
	if (health < 0)
	{
		health = 0;
		Die();
	}
}

bool Entity::IsDead()
{
	return isDead;
}

void Entity::Die()
{
	this->isDead = true;
	this->active = false;
}

unsigned int Entity::GetHP()
{
	return this->health;
}