#include "SnakeHead.h"
#include "Controls.h"
#include "MeshManager.h"
#include "Mtx44.h"
#include "Player.h"
#include "Particle.h"

SnakeHead::SnakeHead()
{
}

SnakeHead::~SnakeHead()
{
}

void SnakeHead::Init(Vector3 pos, unsigned bodyCount)
{
	Enemy::Init(pos);
	team = TEAM_ENEMY;
	entityType = Entity::ENTITY_BOSS_MAIN;
	this->collider.type = Collider::COLLIDER_BALL;
	//scale;
	this->speedLimit = 30.0f;
	this->movementSpeed = 60.0f;
	//health;
	//captureRatio;
	actionRate = ACTION_TIMER_MAX;

	health = 3000;
	maxHealth = health;
	this->mass = 10.0f;

	isRecovering = false;
	isRaging = false;

	this->maxBodyCount = bodyCount;

	for (int i = 0; i < bodyCount; ++i)
	{
		SnakeBody* body = new SnakeBody();
		body->Init(this->pos, movementSpeed * 10.0f, speedLimit);
		GameObject::goList.push_back(body);
		bodyList.push_back(body);
	}
}

void SnakeHead::Update(double dt)
{
	Enemy::Update(dt);

	float bodyRatio = (float)bodyList.size() / (float)maxBodyCount;//counts the percentage of body left
	float headLimit = speedLimit * bodyRatio;
	float headSpeed = movementSpeed * bodyRatio;

	if (isRaging)
	{
		headSpeed = movementSpeed * RAGING_SPEED_MULTIPLIER;
		headLimit = speedLimit * RAGING_SPEED_MULTIPLIER;
	}

	if (!isRecovering)
	{
		if (this->vel.IsZero() == false)
			this->front = vel.Normalized();
		if (target->pos != this->pos)
		{
			this->vel += (target->pos - this->pos).Normalized() * headSpeed * dt;
			if (this->vel.LengthSquared() > headLimit * headLimit)
				this->vel = this->vel.Normalized() * headLimit;
		}
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
		static bool emitSignal = true;
		actionRate -= dt;
		if (actionRate <= 0.5f && emitSignal)
		{
			emitSignal = false;
			EmitSignalParticle(this->pos, this->scale.x, this->scale.x * 7);
		}
		if (actionRate <= 0.0f)
		{
			emitSignal = true;
			actionRate = Math::RandFloatMinMax(ACTION_TIMER_MIN + (10 * (1 - bodyRatio)), ACTION_TIMER_MAX + (10 * (1 - bodyRatio)));
			Action();
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
				(*it)->SetMovementSpeed(movementSpeed);
				(*it)->SetSpeedLimit(speedLimit);
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
				this->health += BODY_PICKUP_RECOVER_AMOUNT;
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
		if (b->GetType() == GameObject::GO_ENTITY)
		{
			if (CheckCollision(b, dt))//if touching dead body part
			{
				Player* player = dynamic_cast<Player*>(b);
				if (player)
					player->TakeDamage(ATTACK_RAM_DAMAGE);
				else
				{
					Enemy* enemy = static_cast<Enemy*>(b);
					enemy->TakeDamage(ATTACK_RAM_DAMAGE);
				}

				CollisionResponse(b);
			}
		}

		GameObject::HandleInteraction(b, dt);
	}
}

void SnakeHead::Action()
{
	float bodyRatio = (float)bodyList.size() / (float)maxBodyCount;
	float healthRatio = (float)health / (float)maxHealth;
	float chanceToAttack = Math::Min(bodyRatio, healthRatio);//higher it is, more likely to attack
	float rand = Math::RandFloatMinMax(0, 1);
	if (rand <= chanceToAttack || healthRatio > 0.6f)//attack
	{
		bool chance = !(bool)(Math::RandInt() % 5);//20% chance to use enrage
		std::cout << chance << std::endl;
		if (isCapturing || chance)
			Enrage();
		else
			Shoot();
	}
	else//recover
	{
		//if lower health than lower body
		if (healthRatio < bodyRatio && healthRatio > 0.2f)
		{
			for (int i = 0; i < bodyList.size(); ++i)
			{
				if (Math::RandInt() % 2)//50% percent chance to discard body
				{
					bodyList[i]->ApplyForce(bodyList[i]->pos - this->pos, 50.0f);
					bodyList[i]->Die();
				}
			}
		}
		else
		{
			Recover();
		}
	}
}

void SnakeHead::Recover()
{
	isRecovering = true;
	actionLifetime = 2.0f + 10 * (1 - (bodyList.size() / maxBodyCount));
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
	actionLifetime = 6.0f;
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

void SnakeHead::Captured()
{
	isCaptured = true;
	Die();
}

void SnakeHead::SetupMesh()
{
	float degree = Math::RadianToDegree(atan2(front.y, front.x));
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	this->mesh = meshList[GEO_SNAKE_HEAD];
}
