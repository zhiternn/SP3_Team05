#include "SnakeBody.h"
#include "ProjectileList.h"
#include "MeshManager.h"

SnakeBody::SnakeBody():
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
}

void SnakeBody::Die()
{
	isDead = true;
	team = GameObject::TEAM_NEUTRAL;
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
	if (backLink)
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
	static float pitchDegree = 0.0f;

	Vector3 right = front.Cross(Vector3(0, 0, 1));
	float yawDegree = Math::RadianToDegree(atan2(front.y, front.x));
	pitchDegree = Math::Wrap(pitchDegree + vel.Length(), 0.0f, 360.0f);

	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(yawDegree, 0, 0, 1);
	modelStack.Rotate(pitchDegree, right.x, right.y, right.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	mesh = meshList[GEO_SNAKE_BODY];
}