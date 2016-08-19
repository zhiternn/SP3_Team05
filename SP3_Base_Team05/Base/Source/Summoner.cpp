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
	speedLimit = 30.0f;
	movementSpeed = 40.0f;
	scale.Set(10, 10, 10);
	safetyThreshold = this->GetScale().x * 5;
	chaseThreshold = safetyThreshold * 1.5f;
	for (int i = 0; i < 6; ++i)
	{
		Summons* summons = new Summons();
		Vector3 offset(Math::RandFloatMinMax(0, 5), Math::RandFloatMinMax(0, 5), 0);
		summons->Init(pos + offset);
		summons->SetTarget(this);
		summonsList.push_back(summons);
		GameObject::goList.push_back(summons);
	}
	for (auto q : summonsList)
	{
		q->Init(this->pos);
		q->SetTarget(this);
	}

	captureRatio = 1.f;
}

void Summoner::Update(double dt)
{
	GameObject::Update(dt);
	Defend();
	if (!Enemy::UpdateMovement(dt))
	{
		float distanceFromTarget = (target->pos - pos).LengthSquared();
		if (distanceFromTarget < safetyThreshold * safetyThreshold ||
			distanceFromTarget > chaseThreshold * chaseThreshold)
		{
			if (target)
			{
				this->front.Set(target->GetPosition().x * (this->scale.x), target->GetPosition().y * (this->scale.y), 0);
				float offset = Math::RandFloatMinMax(safetyThreshold, chaseThreshold);
				Vector3 offsetDir = target->pos - pos;

				Vector3 finalDestination = target->pos - offsetDir.Normalized() * offset;
				ChangeDestination(MOVETO_TARGET, finalDestination);
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

void Summoner::UpdateSummons(double dt)
{
	//for (auto q : summonsList)
	//{
	//	q->
	//}
}

void Summoner::Defend()
{
	if (!summonsList.empty())
	{
		Vector3 N = (target->pos - this->pos).Normalized();
		Vector3 NP = Vector3(-N.y, N.x, 0);
		float diameter = summonsList.front()->GetScale().x * 2;
		float wallLength = summonsList.size()/2 * diameter;
		wallLength = -wallLength / 2;

		Vector3 bossFront;
		bossFront.Set(
			this->pos.x + N.x * (diameter + this->scale.x),
			this->pos.y + N.y * (diameter + this->scale.y),
			0);

		for (int i = 0; i < summonsList.size()/2; ++i)
		{
			float formingWallLength = diameter * i;
			summonsList[i]->Goto(bossFront + (NP * (wallLength + formingWallLength)));
			if (summonsList[i]->IsDead())
			{
				summonsList.erase(summonsList.begin() + i);
			}
		}
	}
}

void Summoner::Attack()
{
	if (!summonsList.empty())
	{
		Vector3 N = (target->pos - this->pos).Normalized();
		Vector3 NP = Vector3(-N.y, N.x, 0);
		float diameter = summonsList.front()->GetScale().x * 2;
		float wallLength = summonsList.size() * diameter;
		wallLength = -wallLength / 2;

		Vector3 bossFront;
		bossFront.Set(
			this->pos.x + N.x * (diameter + this->scale.x),
			this->pos.y + N.y * (diameter + this->scale.y),
			0);

		for (int i = 0; i < summonsList.size(); ++i)
		{
			float formingWallLength = diameter * i;
			summonsList[i]->Goto(bossFront + (NP * (wallLength + formingWallLength)));
			if (summonsList[i]->IsDead())
			{
				summonsList.erase(summonsList.begin() + i);
			}
		}
	}
}