#include "Entity.h"

Entity::Entity(ENTITY_TYPE type) :
entityType(type),
GameObject(GameObject::GO_ENTITY),
health(100)
{
}

Entity::~Entity()
{
}

void Entity::TakeDamage(unsigned amount)
{
	if (!isDead)
	{
		health -= amount;
		if (health < 0)
		{
			health = 0;
			Die();
		}
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

short int Entity::GetHP()
{
	return this->health;
}

short int Entity::GetMaxHP()
{
	return this->maxHealth;
}

void Entity::SetHP(short int HP)
{
	this->health = HP;
}

Entity::ENTITY_TYPE Entity::GetEntityType()
{
	return entityType;
}

void Entity::SetEntityType(ENTITY_TYPE type)
{
	this->entityType = type;
}