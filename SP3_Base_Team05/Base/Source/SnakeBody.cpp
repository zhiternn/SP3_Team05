#include "SnakeBody.h"

SnakeBody::SnakeBody():
Enemy(),
link(NULL)
{
}

SnakeBody::~SnakeBody()
{
	if (link)
		delete link;
}

void SnakeBody::Update(double dt)
{
	GameObject::Update(dt);
	if (!Enemy::UpdateMovement(dt))
	{
		if (link)
		{
			if (pos != link->pos)
			{
				Vector3 toLink = (link->pos - pos).Normalized();

				AddDestination(link->pos - (toLink * link->GetScale().x + scale.x));
			}
		}
	}
	if (vel.LengthSquared() > 10.0f * 10.0f)
	{
		vel = vel.Normalized() * 10.0f;
	}
}

void SnakeBody::LinkTo(Entity* entity)
{
	link = entity;
}