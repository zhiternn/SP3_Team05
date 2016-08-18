#include "Enemy.h"

Enemy::Enemy():
Entity(),
target(NULL)
{
}

Enemy::~Enemy()
{
	if (target)
		delete target;
}

void Enemy::Init(Vector3 pos)
{
	this->pos = pos;
	this->active = true;
	destinationCountdown = REACH_CHECKER;
	team = TEAM_ENEMY;
}

void Enemy::Update(double dt)
{
	GameObject::Update(dt);
	if (vel.IsZero() == false)
		front = vel.Normalized();

	if (!UpdateMovement(dt))
	{//if fail to update (zero destination left)
	}
}

bool Enemy::UpdateMovement(double dt)
{
	if (destinationCountdown > 0)
		destinationCountdown -= dt;
	if (destinations.size() > 0)
	{
		if (Reached(destinations.back()))
		{
			if (destinations.size() > 0)
				destinations.pop_back();
		}
		else if (destinationCountdown <= 0)
		{
			destinationCountdown = REACH_CHECKER;
			destinations.pop_back();
		}
		
		else
		{
			Vector3 dir = (destinations.back() - pos).Normalized();

			vel += dir * movementSpeed;

			if (vel.LengthSquared() > speedLimit * speedLimit)
			{
				vel = vel.Normalized() * speedLimit;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

Vector3 Enemy::FindNewPath(Vector3 destination, GameObject* obstacle)
{
	switch (obstacle->GetCollider().type)
	{
	case Collider::COLLIDER_BALL:
	{
		Vector3 toObstacle = (obstacle->pos - pos).Normalized();
		Vector3 NP = Vector3(-toObstacle.y, toObstacle.x, toObstacle.z);
		float offset = obstacle->GetScale().x + (scale.x * 1.5f);

		return obstacle->pos + (NP * offset);
	}
		break;
	case Collider::COLLIDER_BOX:
	{
		Vector3 toDestination = destination - pos;
		Vector3 toObstacle = obstacle->pos - pos;
		Vector3 pointOnBox = obstacle->pos;
		Vector3 N = obstacle->GetFront();
		Vector3 NP = Vector3(-N.y, N.x, N.z);

		if (toObstacle.Dot(NP) < 0)
			NP = -NP;
		if (toObstacle.Dot(N) < 0)
			N = -N;

		pointOnBox += (toObstacle.Dot(N) * N) + (toObstacle.Dot(NP) * NP);

		if (toObstacle.Dot(N) < toObstacle.Dot(NP))
		{
			float offset = (toObstacle.Dot(N) - obstacle->GetScale().x) + this->scale.x;
			return obstacle->pos + (NP * obstacle->GetScale().y) + (N * offset);
		}
		else
		{
			float offset = (toObstacle.Dot(NP) - obstacle->GetScale().y) + this->scale.x;
			return obstacle->pos + (N * obstacle->GetScale().x) + (NP * offset);
		}

	}
		break;

	default:
		break;
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
	this->destinations.push_back(pos);
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

void Enemy::SetRate(float rate)
{
	//Clamp the captureRatio
	Math::Clamp(rate, 0.f, 1.f);
	captureRatio = rate;
}

void Enemy::SetTarget(Entity* target)
{
	this->target = target;
}

float Enemy::GetRate()
{
	return this->captureRatio;
}
