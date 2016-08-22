#include "Summoner.h"
#include "Controls.h"
#include "MeshManager.h"

using namespace std;

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
	health = 300;
	maxHealth = health;
	isDead = false;
	agressiveLevel = 1 - ((float)health / maxHealth);
	safetyThreshold = this->GetScale().x * 7;
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
	if (summonsList.size() < 5)
	{
		Summons* summons = new Summons();
		summons->Init(this->pos);
		summons->SetTarget(this);
		summonsList.push_back(summons);
		GameObject::goList.push_back(summons);
	}
	Defend();
	Attack();
	for (auto q : summonsList)
	{
		if (!q->isDefending)
		{
			if (Controls::GetInstance().OnHold(Controls::KEY_V))
			{
				q->Shoot(target->pos);
			}
		}
	}
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

void Summoner::TakeDamage(unsigned amount)
{
	Entity::TakeDamage(amount);
	agressiveLevel = 1 - ((float)health / maxHealth);
}

void Summoner::CleaningUpMess()
{
	for (int i = 0; i < summonsList.size(); ++i)
	{
		if (summonsList[i]->IsDead())
		{
			summonsList.erase(summonsList.begin() + i);
		}
	}
}

void Summoner::Defend()
{
	CleaningUpMess();
	if (!summonsList.empty())
	{
		Vector3 N = (target->pos - this->pos).Normalized();
		Vector3 NP = Vector3(-N.y, N.x, 0);
		float diameter = summonsList.front()->GetScale().x * 2;
		float wallLength = ((summonsList.size() * agressiveLevel) - 1) * diameter;
		wallLength = -wallLength * 0.5f;

		Vector3 bossFront;
		bossFront.Set(
			this->pos.x + N.x * (diameter + this->scale.x),
			this->pos.y + N.y * (diameter + this->scale.y),
			0);

		for (int i = 0; i < summonsList.size() * agressiveLevel; ++i)
		{
			float formingWallLength = diameter * i;
			summonsList[i]->Goto(bossFront + (NP * (wallLength + formingWallLength)));
			summonsList[i]->isDefending = true;
		}
	}
}

void Summoner::Attack()
{
	CleaningUpMess();
	if (!summonsList.empty())
	{
		for (auto q : summonsList)
		{
			if (!q->isDefending)
			{
				float combinedRadius = scale.x + target->GetScale().x;
				float offset = combinedRadius * 5;
				//Vector3 offsetDir(
				//	Math::RandFloatMinMax(-(target->pos.x - pos.x), target->pos.x - pos.x),
				//	Math::RandFloatMinMax(-(target->pos.y - pos.y), target->pos.y - pos.y),
				//	0);
				Vector3 offsetDir(target->pos.x - pos.x, target->pos.y - pos.y, 0);

				Vector3 destination = target->pos + offsetDir.Normalized() * offset;
				q->Goto(destination);
			}
		}
	}
}

void Summoner::SetupMesh()
{
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_SPHERE];
}