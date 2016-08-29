#include "Enemy.h"
#include "Trap.h"
#include "MeshManager.h"

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
	this->isDead = false;
	this->isCaptured = false;
	this->isCapturing = false;
	destinationCountdown = REACH_CHECKER;
	type = GameObject::GO_ENTITY;
	team = TEAM_ENEMY;
	captureRate = 0;
	this->health = 100;
	this->collider.type = Collider::COLLIDER_BALL;

	this->movementSpeed = 60.0f;
	this->speedLimit = 15.0f;
}

void Enemy::Update(double dt)
{
	GameObject::Update(dt);
	if (isCapturing)
	{
		Capturing(dt);
		isCapturing = false;
	}
	else
	{
		captureRate = 0;
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

			vel += dir * movementSpeed * dt;

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
		Vector3 toDestination = destination - pos;
		Vector3 toDestinationP = Vector3(-toDestination.y, toDestination.x, 0).Normalized();
		Vector3 toObstacle = obstacle->pos - pos;

		if (toObstacle.Dot(toDestinationP) > 0)
			toDestinationP = -toDestinationP;

		float NPoffset = obstacle->GetScale().x + this->scale.x * 1.5f;
		return obstacle->pos + (toDestinationP.Normalized() * NPoffset);
	}
		break;
	case Collider::COLLIDER_BOX:
	{
		Vector3 toObstacle = destination - pos;
		Vector3 N = obstacle->GetFront();
		Vector3 NP = Vector3(-N.y, N.x, 0);

		if (toObstacle.Dot(N) > 0)
			N = -N;
		if (toObstacle.Dot(NP) > 0)
			NP = -NP;

		float Noffset = obstacle->GetScale().x * 0.5f + this->scale.x * 1.5f;
		float NPoffset = obstacle->GetScale().y * 0.5f + this->scale.x * 1.5f;

		return obstacle->pos + (N * Noffset) + (NP * NPoffset);
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

void Enemy::ChangeDestination(Enemy::MOVEMENT_PRIORITY priority, Vector3 pos)
{
	if (destinations.size() < priority + 1)
		this->destinations.push_back(pos);
	else
		this->destinations[priority] = pos;
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

void Enemy::SetTarget(Entity* target)
{
	this->target = target;
}

void Enemy::HandleInteraction(GameObject* b, double dt)
{
	if (b->GetType() == GameObject::GO_ENVIRONMENT)
	{
		Trap* trap = dynamic_cast<Trap*>(b);
		if (trap)
		{	
			Vector3 dir = Vector3(1, 0, 0);
			if (this->pos != trap->pos)
				dir = (this->pos - trap->pos).Normalized();
			this->vel += dir * speedLimit * dt;
		}
	}

	GameObject::HandleInteraction(b, dt);
}
void Enemy::SetSpeedLimit(float speed)
{
    this->speedLimit = speed;
}

void Enemy::SetMovementSpeed(float speed)
{
    this->movementSpeed = speed;
}

float Enemy::GetSpeedLimit()
{
    return this->speedLimit;
}

float Enemy::GetMovementSpeed()
{
    return this->movementSpeed;
}

void Enemy::SetCaptureRate(float captureRate)
{
	this->captureRate = captureRate;
}

float Enemy::GetCaptureRate()
{
	return captureRate;
}

void Enemy::SetCaptured(bool isCaptured)
{
	this->isCaptured = isCaptured;
}

bool Enemy::IsCaptured()
{
	return isCaptured;
}

void Enemy::SetCapturing(bool isCapturing)
{
	this->isCapturing = isCapturing;
}

bool Enemy::IsCapturing()
{
	return isCapturing;
}

void Enemy::Capturing(double dt)
{
	captureRate += dt * 20;
	if (captureRate >= health / 20)
	{
		Captured();
	}
}

void Enemy::Captured()
{
	this->isCaptured = true;
	this->active = false;
	
	// do the pokemon things
}

void Enemy::SetupMesh()
{
	float degree = Math::RadianToDegree(atan2(front.y, front.x));
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	mesh = meshList[GEO_SPHERE];
}