#include "Summons.h"

Summons::Summons()
{

}

Summons::~Summons()
{
}

void Summons::Init(Vector3 pos)
{
	Enemy::Init(pos);
	type = GameObject::GO_ENTITY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	speedLimit = 30.f;
	movementSpeed = 30.f;
	scale.Set(5, 5, 5);
	health = 10;
	isDead = false;
	innerThreshold = this->GetScale().x;
	outerThreshold = innerThreshold * 1.5f;
}

void Summons::Update(double dt)
{
	GameObject::Update(dt);

	if (vel.LengthSquared() > speedLimit * speedLimit)
		vel = vel.Normalized() * speedLimit;
	//if (!Enemy::UpdateMovement(dt))
	//{
	//	if (target)
	//	{
	//		float distanceFromTarget = (target->GetPosition() - pos).LengthSquared();
	//		if (distanceFromTarget < innerThreshold * innerThreshold ||
	//			distanceFromTarget > outerThreshold * outerThreshold)
	//		{
	//			if (target)
	//			{
	//				if (target->GetPosition() != pos)
	//				{
	//					Vector3 offsetDir = target->GetPosition() - pos;
	//					offsetDir.Set(-offsetDir.y, offsetDir.x, offsetDir.z);

	//					Vector3 finalDestination = target->GetPosition() - offsetDir.Normalized() * 30;
	//					ChangeDestination(MOVETO_TARGET, finalDestination);
	//				}
	//			}
	//		}
	//	}
	//}
}

void Summons::Goto(Vector3 pos)
{
	this->vel += (pos - this->pos).Normalized() * movementSpeed;
}