#include "SnakeHead.h"
#include "SnakeBody.h"

SnakeHead::SnakeHead():
target(NULL)
{
	SnakeBody* body = new SnakeBody();
	body->LinkTo(this);
	body->SetScale(2, 2, 2);
	body->SetPostion(Math::RandFloatMinMax(350, 370), Math::RandFloatMinMax(230, 270), 0);
	body->SetActive(true);
	GameObject::goList.push_back(body);

	SnakeBody* prevBody = body;

	for (int i = 0; i < 5; ++i)
	{
		SnakeBody* body2 = new SnakeBody();
		body2->LinkTo(prevBody);
		body2->SetActive(true);
		body2->SetScale(2, 2, 2);
		body->SetPostion(Math::RandFloatMinMax(350, 370), Math::RandFloatMinMax(230, 270), 0);
		GameObject::goList.push_back(body2);
		prevBody = body2;
	}
}

SnakeHead::~SnakeHead()
{
	if (target)
		delete target;
}

void SnakeHead::Update(double dt)
{
	GameObject::Update(dt);

	if (!Enemy::UpdateMovement(dt))
	{
		if (target)
		{
			float offset = Math::RandFloatMinMax(5, 15);
			Vector3 offsetDir = target->pos - pos;
			offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

			Vector3 destination = target->pos + offsetDir.Normalized() * offset;
			AddDestination(destination);

			//{//testing stuff
			//	static GameObject* hehe = NULL;
			//	if (hehe)
			//		hehe->SetActive(false);

			//	GameObject* go = FetchGO();
			//	hehe = go;
			//	go->SetActive(true);
			//	go->SetVelocity(0, 0, 0);
			//	go->pos = destination;
			//}
		}
		else
		{
			Vector3 temp(Math::RandFloatMinMax(200, 250), Math::RandFloatMinMax(200, 250), 0);
			AddDestination(temp);
		}
	}
}

void SnakeHead::HandleInteraction(GameObject* b, double dt)
{
	if (CheckCollision(b, dt))
	{
		CollisionResponse(b);
	}
}

void SnakeHead::SetTarget(Entity* target)
{
	this->target = target;
}