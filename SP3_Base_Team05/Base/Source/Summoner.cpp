#include "Summoner.h"

Summoner::Summoner()
{
}

Summoner::~Summoner()
{
}

void Summoner::Init(Vector3 pos)
{
	Enemy::Init(pos);
	type = GameObject::GO_ENTITY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	speedLimit = 20.f;
	movementSpeed = 20.0f;
	scale.Set(10, 10, 10);
	safetyThreshold = 100;
	chaseThreshold = safetyThreshold * 1.5f;

	captureRatio = 1.f;
}

void Summoner::Update(double dt)
{
	GameObject::Update(dt);
	
	if (!Enemy::UpdateMovement(dt))
	{
		float distanceFromTarget = (target->pos - pos).LengthSquared();
		if (distanceFromTarget < safetyThreshold * safetyThreshold ||
			distanceFromTarget > chaseThreshold * chaseThreshold)
		{
			if (target)
			{
				float offset = Math::RandFloatMinMax(safetyThreshold, chaseThreshold);
				Vector3 offsetDir = target->pos - pos;

				Vector3 finalDestination = target->pos - offsetDir.Normalized() * offset;
				AddDestination(finalDestination);
			}
		}
		else
		{
			vel.SetZero();
		}
	}
}

void Summoner::HandleInteraction(GameObject* b, double dt)
{

}