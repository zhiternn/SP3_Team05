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
	speedLimit = 20;
	movementSpeed = 20.0f;
	scale.Set(10, 10, 10);
	safetyThreshold = this->GetScale().x * 5;
	chaseThreshold = safetyThreshold * 1.5f;
	for (int i = 0; i < 5; ++i)
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
	int defendee = 5;
	Vector3 N = (target->pos - this->pos).Normalized();
	Vector3 NP = Vector3(-N.y, N.x, 0);
	float diameter = 5 * 2;
	float wallLength = defendee * diameter;
	wallLength = -wallLength / 2;

	Vector3 bossFront = this->pos + (N * this->scale.x + 5);
	
	for (int i = 0; i < defendee; ++i)
	{
		float formingWallLength = diameter * i;

		summonsList[i]->Goto(bossFront + (NP * (wallLength + formingWallLength)));
		//std::cout << (NP * (wallLength + formingWallLength)) << std::endl;
	}
}