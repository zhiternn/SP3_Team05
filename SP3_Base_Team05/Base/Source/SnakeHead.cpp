#include "SnakeHead.h"
#include "Controls.h"
#include "MeshManager.h"
#include "Player.h"

SnakeHead::SnakeHead():
backLink(NULL)
{
}

SnakeHead::~SnakeHead()
{
	if (backLink)
		delete backLink;
}

void SnakeHead::Init(Vector3 pos, unsigned bodyCount)
{
	Enemy::Init(pos);
	entityType = Entity::ENTITY_BOSS_MAIN;
	//scale;
	this->maxBodyCount = bodyCount;
	this->speedLimit = 20.0f;
	this->movementSpeed = 40.0f;
	//health;
	//captureRatio;
	actionTimer = ATTACK_TIMER_MAX;
	health = 2000;

	if (bodyCount > 0)
	{
		SnakeBody* body = new SnakeBody();
		body->SetScale(4, 4, 4);
		body->SetTarget(this);
		body->Init(pos, movementSpeed * 0.25f, speedLimit * 0.75f);
		GameObject::goList.push_back(body);
		backLink = body;

		SnakeBody* prev = body;
		for (int i = 0; i < bodyCount - 1; ++i)
		{
			SnakeBody* body2 = new SnakeBody();
			body2->SetScale(4, 4, 4);
			body2->SetTarget(this);
			body2->Init(pos, movementSpeed * 0.25f, speedLimit * 0.75f);
			GameObject::goList.push_back(body2);
			prev->LinkBackTo(body2);
			prev = body2;
		}
	}
}

void SnakeHead::Update(double dt)
{
	GameObject::Update(dt);
	if (vel.IsZero() == false)
		front = vel.Normalized();

	if (!isDead)
	{
		if (!UpdateMovement(dt))
		{
			if (target)
			{
				float combinedRadius = scale.x + target->GetScale().x;
				float offset = Math::RandFloatMinMax(combinedRadius * 3, combinedRadius * 4);
				Vector3 offsetDir = target->pos - pos;
				offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

				Vector3 destination = target->pos + offsetDir.Normalized() * offset;
				ChangeDestination(MOVETO_TARGET, destination);
			}
		}
		if (backLink)
		{
			if (!backLink->IsDead())
				Pull(backLink);
			else if(backLink->GetBackLink())
			{
				Reconnect();
			}
		}

		actionTimer -= dt;
		if (actionTimer <= 0.0f)
		{
			actionTimer = Math::RandFloatMinMax(ATTACK_TIMER_MIN, ATTACK_TIMER_MAX);
			Action();
		}

		static bool hehe = false;

		if (health <= 1000 && hehe == false)
			hehe = true;

		if (hehe)
		{
			Upgrade();
		}
	}
}

void SnakeHead::HandleInteraction(GameObject* b, double dt)
{
	if (CheckCollision(b, dt))
	{
		Player* player = dynamic_cast<Player*>(b);
		if (player)
		{
			player->TakeDamage(ATTACK_RAM_DAMAGE);
		}
		SnakeBody* body = dynamic_cast<SnakeBody*>(b);
		if (body)
		{
			if (body->IsDead())
			{
				SnakeBody* last = GetLast();
				last->LinkBackTo(body);
				body->Init(pos, movementSpeed * 0.25f, speedLimit * 0.75f);
			}
		}

		CollisionResponse(b);
	}
}

void SnakeHead::Action()
{
	if (backLink)
		backLink->Fire(target);
}

void SnakeHead::Die()
{
	isDead = true;
	active = false;

	if (backLink)
	{
		SnakeBody* body = backLink;
		body->Die();

		while (body->GetBackLink() != NULL)
		{
			body = body->GetBackLink();
			body->Die();
		}
	}
}

bool SnakeHead::UpdateMovement(double dt)
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

			float limit = speedLimit * ((float)GetBodyCount() / (float)maxBodyCount);
			if (vel.LengthSquared() > limit * limit)
			{
				vel = vel.Normalized() * limit;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

void SnakeHead::Upgrade()
{
	//this->movementSpeed = 20.f;
	//this->speedLimit = 40.0f;

	//if (backLink)
	//{
	//	SnakeBody* body = backLink;

	//	body->movementSpeed = this->movementSpeed * 0.25f;
	//	body->speedLimit = this->speedLimit * 0.75f;

	//	while (body->GetBackLink() != NULL)
	//	{
	//		body = body->GetBackLink();

	//		body->movementSpeed = this->movementSpeed * 0.25f;
	//		body->speedLimit = this->speedLimit * 0.75f;
	//	}
	//}
}

void SnakeHead::Reconnect()
{
	SnakeBody* link = backLink->GetBackLink();
	backLink->LinkBackTo(NULL);
	backLink = link;
}

void SnakeHead::Pull(SnakeBody* body)
{
	Vector3 point = pos - front * scale.x;
	if (!body->Reached(point))
	{
		body->Goto(point);
	}
}

SnakeBody* SnakeHead::GetLast()
{
	if (backLink)
	{
		SnakeBody* last = backLink;
		while (last->GetBackLink() != NULL)
		{
			if (last == last->GetBackLink())
				break;

			last = last->GetBackLink();
		}
		return last;
	}
	else
	{
		return NULL;
	}
}

int SnakeHead::GetBodyCount()
{
	if (backLink)
	{
		int count = 1;
		SnakeBody* last = backLink;
		while (last->GetBackLink() != NULL)
		{
			last = last->GetBackLink();

			{//for safety
				if (count > maxBodyCount)
					break;
				if (last == last->GetBackLink())
					break;
			}

			count++;
		}

		return count;
	}
	else
		return 0;
}

void SnakeHead::SetupMesh()
{
	float degree = Math::RadianToDegree(atan2(front.y, front.x));
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	this->mesh = meshList[GEO_SNAKE_HEAD];
}
