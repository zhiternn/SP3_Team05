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
}

void SnakeBody::LinkTo(Entity* entity)
{
	link = entity;
}