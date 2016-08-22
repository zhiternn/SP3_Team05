#include "SnakeHead.h"
#include "Controls.h"
#include "MeshManager.h"

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
	scale;
	speedLimit = 20.0f;
	movementSpeed = 40.0f;
	health;
	captureRatio;

	if (bodyCount > 0)
	{
		SnakeBody* body = new SnakeBody();
		body->SetScale(4, 4, 4);
		body->Init(pos, 10, 15);
		GameObject::goList.push_back(body);
		backLink = body;

		SnakeBody* prev = body;
		for (int i = 0; i < bodyCount - 1; ++i)
		{
			SnakeBody* body2 = new SnakeBody();
			body2->SetScale(4, 4, 4);
			body2->Init(pos, 10, 15);
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

	if (!Enemy::UpdateMovement(dt))
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
		else
			Reconnect();
	}

	if (Controls::GetInstance().OnPress(Controls::KEY_L))
	{
		if (backLink)
			backLink->Fire(target);
	}
}

void SnakeHead::Action()
{

}

void SnakeHead::Die()
{
	isDead = true;
	active = false;
}

void SnakeHead::Reconnect()
{
	if (backLink)
	{
		backLink = backLink->GetBackLink();
	}
}

void SnakeHead::Pull(SnakeBody* body)
{
	Vector3 point = pos - front * scale.x;
	if (!body->Reached(point))
	{
		body->Goto(point);
	}
}

void SnakeHead::SetupMesh()
{
	float degree = Math::RadianToDegree(atan2(front.y, front.x));
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	mesh = meshList[GEO_SPHERE];
}