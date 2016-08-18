#include "SnakeBody.h"

SnakeBody::SnakeBody():
Enemy(),
back(NULL)
{
	speedLimit = 15.0f;
	movementSpeed = 30.0f;
	collider.type = Collider::COLLIDER_BALL;
}

SnakeBody::~SnakeBody()
{
	if (back)
		delete back;
}

void SnakeBody::Update(double dt)
{
	GameObject::Update(dt);
	if (vel.IsZero() == false)
		front = vel.Normalized();

	if (vel.LengthSquared() > speedLimit * speedLimit)
	{
		vel = vel.Normalized() * speedLimit;
	}

	if (back)
		Pull(back);
}

void SnakeBody::Pull(SnakeBody* body)
{
	Vector3 point = pos - front * scale.x;
	if (!body->Reached(point))
	{
		body->Goto(point);
	}
}

void SnakeBody::LinkTo(SnakeBody* entity)
{
	back = entity;
}

void SnakeBody::Goto(Vector3 pos)
{
	vel += (pos - this->pos).Normalized() * movementSpeed;
}