#include "SnakeHead.h"
#include "Controls.h"
#include "MeshManager.h"

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
	this->speedLimit = 20.0f;
	this->movementSpeed = 40.0f;
	//health;
	//captureRatio;
	actionRate = ACTION_TIMER_MAX;
	health = 1000;

	this->maxBodyCount = bodyCount;

	for (int i = 0; i < bodyCount; ++i)
	{
		SnakeBody* body2 = new SnakeBody();
		body2->Init(this->pos, movementSpeed * 8.0f, speedLimit);

		GameObject::goList.push_back(body2);
		bodyList.push_back(body2);
	}
}

void SnakeHead::Update(double dt)
{
	GameObject::Update(dt);
	
	if (this->vel.IsZero() == false)
		this->front = vel.Normalized();

	this->vel += (target->pos - this->pos).Normalized() * movementSpeed * dt;
	
	float movementLimit = speedLimit * ((double)bodyList.size() / (double)maxBodyCount);
	if (this->vel.LengthSquared() > movementLimit * movementLimit)
		this->vel = this->vel.Normalized() * movementLimit;

	{//Move Body
		Enemy* prev = this;

		std::vector<SnakeBody*>::iterator it;
		for (it = bodyList.begin(); it != bodyList.end();)
		{
			SnakeBody* body = (*it);
			if (!body->IsDead())// if its not dead
			{
				Vector3 destination = prev->pos - prev->GetFront() * prev->GetScale().x;
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

	if (Controls::GetInstance().OnPress(Controls::KEY_L))
	{
		Shoot();
	}
}

void SnakeHead::HandleInteraction(GameObject* b, double dt)
{
	SnakeBody* body = dynamic_cast<SnakeBody*>(b);
	if (body)
	{
		if (body->IsDead())//if touching dead body part
		{
			if (CheckCollision(b, dt))
			{
				body->Init(body->pos, body->movementSpeed, body->speedLimit);
				bodyList.push_back(body);
			}

			//if is charging
			//body->GoTo(this->pos, dt);
		}
	}
	else
		GameObject::HandleInteraction(b, dt);
}

void SnakeHead::Action()
{
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

void SnakeHead::Die()
{
}

void SnakeHead::SetupMesh()
{
	float degree = Math::RadianToDegree(atan2(front.y, front.x));
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	this->mesh = meshList[GEO_SNAKE_HEAD];
}
