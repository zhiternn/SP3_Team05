#include "Enemy.h"

Enemy::Enemy():
GameObject(GameObject::GO_ENEMY)
{
}

Enemy::~Enemy()
{
}

void Enemy::UpdateMovement(double dt)
{
	if (destinations.size() > 0)
	{
		Vector3 dir = (destinations.top() - pos).Normalized();
		if (Reached(destinations.top()))
		{
			destinations.pop();
		}
		else
		{
			vel = dir * 10;
		}
	}
	else
	{
		Vector3 temp = Vector3(Math::RandFloatMinMax(10, 50), Math::RandFloatMinMax(10, 50), 0);
		AddDestination(temp);
	}
}

bool Enemy::Reached(Vector3 pos)
{
	float distance = (this->pos - pos).LengthSquared();

	if (distance <= (scale.x * scale.x))
		return true;
	else
		return false;
}

void Enemy::AddDestination(Vector3 pos)
{
	this->destinations.push(pos);
}

Enemy* FetchEnemy()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Enemy *enemy = dynamic_cast<Enemy*>((*it));
		if (enemy && enemy->GetActive() == false)
		{
			enemy->GameObject::SetType(GameObject::GO_PROJECTILE);
			enemy->SetActive(true);
			return enemy;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new Enemy());
	}
	Enemy *enemy = dynamic_cast<Enemy*>(*(GameObject::goList.end() - 10));
	if (enemy)
	{
		enemy->GameObject::SetType(GameObject::GO_PROJECTILE);
		enemy->SetActive(true);
		return enemy;
	}

   { //for safety measure
	   Enemy *enemy = new Enemy();
	   enemy->GameObject::SetType(GameObject::GO_PROJECTILE);
	   enemy->SetActive(true);
	   GameObject::goList.push_back(enemy);
	   return enemy;
   }
}