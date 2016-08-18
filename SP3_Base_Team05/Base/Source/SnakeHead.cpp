#include "SnakeHead.h"

SnakeHead::SnakeHead():
back(NULL)
{
}

SnakeHead::~SnakeHead()
{
	if (back)
		delete back;
}

void SnakeHead::Init(Vector3 pos)
{
	Enemy::Init(pos);
	scale;
	speedLimit = 30.0f;
	movementSpeed = 60.0f;
	health;
	captureRatio;

	SnakeBody* body = new SnakeBody();
	body->SetScale(4, 4, 4);
	body->Init(pos);
	GameObject::goList.push_back(body);
	back = body;

	SnakeBody* prev = body;
	for (int i = 0; i < 20; ++i)
	{
		SnakeBody* body2 = new SnakeBody();
		body2->SetScale(4, 4, 4);
		body2->Init(pos);
		GameObject::goList.push_back(body2);
		prev->LinkTo(body2);
		prev = body2;
	}
}

void SnakeHead::Update(double dt)
{
	GameObject::Update(dt);
	if (vel.IsZero() == false)
		front = vel.Normalized();

	if (!Enemy::UpdateMovement(dt))
	{
		if (target)
		{
			float combinedRadius = scale.x + target->GetScale().x;
			float offset = Math::RandFloatMinMax(combinedRadius * 2, combinedRadius * 3);
			Vector3 offsetDir = target->pos - pos;
			offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

			Vector3 destination = target->pos + offsetDir.Normalized() * offset;
			ChangeDestination(MOVETO_TARGET, destination);

			{//testy stuff
				static GameObject* hehe = NULL;
				GameObject* go = FetchGO();
				go->SetActive(true);
				go->SetScale(1, 1, 1);

				go->pos = destination;
				if (hehe)
					hehe->SetActive(false);
				hehe = go;
			}
		}
	}
	if (back)
		Pull(back);
}

void SnakeHead::Pull(SnakeBody* body)
{
	Vector3 point = pos - front * scale.x;
	if (!body->Reached(point))
	{
		body->Goto(point);
	}
}