#include "Entity.h"

Entity::Entity():
GameObject(GameObject::GO_ENTITY),
health(3)
{
}

Entity::~Entity()
{
}

void Entity::TakeDamage(unsigned amount)
{
	health -= amount;
	std::cout << health << std::endl;
	if (health < 0)
	{
		health = 0;
		isDead = true;
		active = false;
	}
}

bool Entity::IsDead()
{
	return isDead;
}

unsigned int Entity::GetHP()
{
	return this->health;
}