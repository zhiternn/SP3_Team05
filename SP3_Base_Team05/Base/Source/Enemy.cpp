#include "Enemy.h"

Enemy::Enemy():
Entity()
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(Vector3 pos)
{
	this->pos = pos;
	active = true;
	type = GameObject::GO_ENTITY;
	team = TEAM_ENEMY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	checkReached = REACH_CHECKER;
	speedLimit = 10.f;
}

void Enemy::Update(double dt)
{
	GameObject::Update(dt);
	if (!UpdateMovement(dt))
	{//if fail to update (zero destination left)
		Vector3 temp(Math::RandFloatMinMax(200, 250), Math::RandFloatMinMax(200, 250), 0);
		AddDestination(temp);
	}
}

void Enemy::HandleInteraction(GameObject* b, double dt)
{
}

bool Enemy::UpdateMovement(double dt)
{
	checkReached -= dt;

	if (destinations.size() > 0)
	{
		Vector3 dir = (destinations.top() - pos).Normalized();
		if (Reached(destinations.top()) || checkReached <= 0)
		{
			checkReached = REACH_CHECKER;
			destinations.pop();
		}
		else
		{
			this->ApplyForce(dt, dir, ENEMY_MOVEMENT_SPEED);
			if (vel.LengthSquared() > speedLimit * speedLimit)
			{
				vel = vel.Normalized() * speedLimit;
			}
		}
		if (vel.IsZero() == false)
			front = vel.Normalized();

		return true;
	}
	else
	{
		return false;
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
		if (enemy && enemy->IsActive() == false)
		{
			enemy->GameObject::SetType(GameObject::GO_ENTITY);
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
		enemy->GameObject::SetType(GameObject::GO_ENTITY);
		enemy->SetActive(true);
		return enemy;
	}

   { //for safety measure
	   Enemy *enemy = new Enemy();
	   enemy->GameObject::SetType(GameObject::GO_ENTITY);
	   enemy->SetActive(true);
	   GameObject::goList.push_back(enemy);
	   return enemy;
   }
}
