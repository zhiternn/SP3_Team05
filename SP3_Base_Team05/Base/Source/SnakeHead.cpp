#include "SnakeHead.h"
#include "Controls.h"
#include "MeshManager.h"
#include "Mtx44.h"
#include "Player.h"

SnakeHead::SnakeHead()
{
}

SnakeHead::~SnakeHead()
{
}

void SnakeHead::Init(Vector3 pos, unsigned bodyCount)
{
	Enemy::Init(pos);
	entityType = Entity::ENTITY_BOSS_MAIN;
	this->collider.type = Collider::COLLIDER_BALL;
	//scale;
	this->speedLimit = 30.0f;
	this->movementSpeed = 60.0f;
	//health;
	//captureRatio;
	actionRate = ACTION_TIMER_MAX;
	health = 1000;
	this->mass = 30.0f;

	isRecovering = false;
	isRaging = false;

	this->maxBodyCount = bodyCount;

	for (int i = 0; i < bodyCount; ++i)
	{
		SnakeBody* body2 = new SnakeBody();
		body2->Init(this->pos, movementSpeed * 10.0f, speedLimit);

		GameObject::goList.push_back(body2);
		bodyList.push_back(body2);
	}
}

void SnakeHead::Update(double dt)
{
	GameObject::Update(dt);

	float bodyRatio = (float)bodyList.size() / (float)maxBodyCount;//counts the percentage of body left
	float headLimit = speedLimit * bodyRatio;
	float headSpeed = movementSpeed * bodyRatio;

	if (isRaging)
	{
		headSpeed = movementSpeed * 2.0f;
		headLimit = speedLimit * 2.0f;
	}

	if (!isRecovering)
	{
		if (this->vel.IsZero() == false)
			this->front = vel.Normalized();

		this->vel += (target->pos - this->pos).Normalized() * headSpeed * dt;
		if (this->vel.LengthSquared() > headLimit * headLimit)
			this->vel = this->vel.Normalized() * headLimit;
	}
	else
	{//Recovering state
		headSpeed = movementSpeed;
		headLimit = speedLimit * 2.0f;

		this->vel -= this->vel * 0.3f * dt;//Apply friction to snake head

		Mtx44 rotate;
		rotate.SetToRotation(150.0f * dt, 0, 0, 1);
		this->front = rotate * front;
	}

	if (actionLifetime <= 0.0f)
	{
		actionRate -= dt;
		if (actionRate <= 0.0f)
		{
			actionRate = Math::RandFloatMinMax(ACTION_TIMER_MIN, ACTION_TIMER_MAX);
			Action(bodyRatio);
		}
	}
	else
	{
		actionLifetime -= dt;
		if (actionLifetime <= 0.0f)
		{
			actionLifetime = 0.0f;
			isRecovering = false;
			isRaging = false;
		}
	}

	
	{//Handles bodies
		Enemy* prev = this;

		std::vector<SnakeBody*>::iterator it;
		for (it = bodyList.begin(); it != bodyList.end();)
		{
			SnakeBody* body = (*it);
			if (!body->IsDead())// if its not dead
			{
				Vector3 destination = prev->pos - prev->GetFront() * prev->GetScale().x;
				
				body->SetMovementSpeed(headSpeed * 10.0f);
				body->SetSpeedLimit(headLimit);

				body->GoTo(destination, dt);

				prev = body;
				++it;
			}
			else
			{
				it = bodyList.erase(it);
			}
		}
	}
}

void SnakeHead::HandleInteraction(GameObject* b, double dt)
{
	SnakeBody* body = dynamic_cast<SnakeBody*>(b);
	if (body)
	{
		if (body->IsDead())
		{
			if (CheckCollision(b, dt))//if touching dead body part
			{
				body->Init(body->pos, body->GetMovementSpeed(), body->GetSpeedLimit());
				bodyList.push_back(body);
			}

			if (isRecovering)
			{
				body->GoTo(this->pos, dt);
			}
		}
	}
	else
	{
		Player* player = dynamic_cast<Player*>(b);
		if (player)
			if (CheckCollision(b, dt))
				player->TakeDamage(DAMAGE_ONTOUCH);

		GameObject::HandleInteraction(b, dt);
	}
}

void SnakeHead::Action(float ratio)
{
	float chanceToAttack = ratio;//higher it is, more likely to attack
	float rand = Math::RandFloatMinMax(0, 1);
	if (rand <= ratio)//attack
	{
		if (Math::RandInt() % 2)
			Shoot();
		else
			Enrage();
	}
	else//recover
	{
		Recover();
	}
}

void SnakeHead::Recover()
{
	isRecovering = true;
	actionLifetime = 5.0f * (bodyList.size() / maxBodyCount) * 10;
}

void SnakeHead::Shoot()
{
	std::vector<SnakeBody*>::iterator it;
	for (it = bodyList.begin(); it != bodyList.end(); ++it)
	{
		SnakeBody* body = (*it);
		body->Shoot(target->pos);
	}
}

void SnakeHead::Enrage()
{
	isRaging = true;
	actionLifetime = 8.0f;
}

void SnakeHead::Die()
{
	this->isDead = true;
	this->active = false;
	
	std::vector<SnakeBody*>::iterator it;
	for (it = bodyList.begin(); it != bodyList.end();)
	{
		SnakeBody* body = (*it);
		body->Die();
		it = bodyList.erase(it);
	}
}

void SnakeHead::SetupMesh()
{
	float degree = Math::RadianToDegree(atan2(front.y, front.x));
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	this->mesh = meshList[GEO_SNAKE_HEAD];
}
