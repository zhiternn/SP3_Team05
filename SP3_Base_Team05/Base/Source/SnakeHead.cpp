#include "SnakeHead.h"
#include "SnakeBody.h"

SnakeHead::SnakeHead()
{
	//SnakeBody* body = new SnakeBody();
	//body->LinkTo(this);
	//body->SetScale(2, 2, 2);
	//body->SetPostion(Math::RandFloatMinMax(350, 370), Math::RandFloatMinMax(230, 270), 0);
	//body->SetActive(true);
	//GameObject::goList.push_back(body);

	//SnakeBody* prevBody = body;

	//for (int i = 0; i < 5; ++i)
	//{
	//	SnakeBody* body2 = new SnakeBody();
	//	body2->LinkTo(prevBody);
	//	body2->SetActive(true);
	//	body2->SetScale(2, 2, 2);
	//	body->SetPostion(Math::RandFloatMinMax(350, 370), Math::RandFloatMinMax(230, 270), 0);
	//	GameObject::goList.push_back(body2);
	//	prevBody = body2;
	//}
}

SnakeHead::~SnakeHead()
{
}

void SnakeHead::Init(Vector3 pos)
{
	this->pos = pos;
	active = true;
	type = GameObject::GO_ENTITY;
	team = TEAM_ENEMY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	destinationCountdown = REACH_CHECKER;
	speedLimit = 10.f;
	movementSpeed = 20.0f;

	captureRatio = 1.f;
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
			float offset = Math::RandFloatMinMax(combinedRadius * 1.5f, combinedRadius * 2.0f);
			Vector3 offsetDir = target->pos - pos;
			offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

			Vector3 destination = target->pos + offsetDir.Normalized() * offset;
			AddDestination(destination);
		}
	}
}