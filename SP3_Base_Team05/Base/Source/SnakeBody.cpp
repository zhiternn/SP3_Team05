#include "SnakeBody.h"
#include "ProjectileList.h"
#include "MeshManager.h"

SnakeBody::SnakeBody():
pitchDegree(0.0f),
Enemy(),
backLink(NULL)
{
}

SnakeBody::~SnakeBody()
{
	if (backLink)
		delete backLink;
}

void SnakeBody::Init(Vector3 pos, float speed, float speedLimit)
{
	Enemy::Init(pos);
	movementSpeed = speed;
	this->speedLimit = speedLimit;
	collider.type = Collider::COLLIDER_BALL;

	weapon = new Splitgun();
	weapon->AssignProjectile(new Bullet());
}

void SnakeBody::Update(double dt)
{
	GameObject::Update(dt);
	if (!isDead)
	{
		weapon->Update(dt);

		if (vel.IsZero() == false)
			front = vel.Normalized();

		if (vel.LengthSquared() > speedLimit * speedLimit)
		{
			vel = vel.Normalized() * speedLimit;
		}

		if (backLink)
		{
			if (!backLink->IsDead())
				Pull(backLink);
			else
				Reconnect();
		}
	}

	//for animation
	pitchDegree += vel.LengthSquared() * dt;//Math::Wrap(pitchDegree + vel.LengthSquared() * (float)dt, 0.0f, 360.0f);
}

void SnakeBody::Die()
{
	isDead = true;
	vel.SetZero();
}

void SnakeBody::Pull(SnakeBody* body)
{
	Vector3 point = pos - front * scale.x;
	if (!body->Reached(point))
	{
		body->Goto(point);
	}
}

void SnakeBody::LinkBackTo(SnakeBody* entity)
{
	backLink = entity;
}

void SnakeBody::Goto(Vector3 pos)
{
	vel += (pos - this->pos).Normalized() * movementSpeed;
}

void SnakeBody::Reconnect()
{
	if (backLink && backLink->GetBackLink())
	{
		backLink = backLink->GetBackLink();
	}
}

void SnakeBody::Fire(Entity* target)
{
	weapon->Fire(this->pos, target->pos - this->pos, team);

	if (backLink)
		backLink->Fire(target);
}

SnakeBody* SnakeBody::GetBackLink()
{
	return backLink;
}

void SnakeBody::SetupMesh()
{
	Vector3 right = front.Cross(Vector3(0, 0, 1));

	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(-pitchDegree, right.x, right.y, right.z);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_SNAKE_BODY];
}