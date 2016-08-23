#include "Summoner.h"
#include "Controls.h"
#include "MeshManager.h"

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
	speedLimit = 50.f;
	movementSpeed = 50.0f;
	scale.Set(15, 15, 15);
	health = 500;
	maxHealth = health;
	isDead = false;
	attacking = false;
	agressiveLevel = 1 - ((float)health / maxHealth);
	safetyThreshold = this->GetScale().x * 7;
	chaseThreshold = safetyThreshold * 1.5f;
	for (int i = 0; i < AMOUNT_OF_SUMMONS; ++i)
	{
		Summons* summons = FetchSummons();
		Vector3 offset(Math::RandFloatMinMax(0, 5), Math::RandFloatMinMax(0, 5), 0);
		summons->Init(pos + offset);
		summons->SetTarget(target);
		summonsList.push_back(summons);
		GameObject::goList.push_back(summons);
	}
	for (auto q : summonsList)
	{
		q->Init(this->pos);
		q->SetTarget(target);
	}
	captureRatio = 1.f;
}

void Summoner::Update(double dt)
{
	GameObject::Update(dt);
	UpdateCooldowns(dt);
	if (summonsList.size() < AMOUNT_OF_SUMMONS && summonCooldownTimer <= 0)
	{
		summonCooldownTimer = SUMMONING_COOLDOWN;
		Summons* summons = FetchSummons();
		summons->Init(this->pos);
		summons->SetTarget(target);
		summonsList.push_back(summons);
		GameObject::goList.push_back(summons);
	}
	if (this->health < maxHealth)
	{
		static float healthregenCooldown = 0;
		healthregenCooldown += dt;
		if (healthregenCooldown >= 1)
		{
			healthregenCooldown = 0.0f;
			if (health + HEALTH_REGEN_PERSEC > maxHealth)
				health = maxHealth;
			else
				health += HEALTH_REGEN_PERSEC;
		}
	}
	std::cout << health << std::endl;
	if (!summonsList.empty())
	{
		Defend();
		Attack();
		for (auto q : summonsList)
		{
			if (!q->isDefending && attacking)
			{
				//q->Shoot(target->pos);
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
		Vector3 N = (target->pos - this->pos).Normalized();
		Vector3 NP = Vector3(-N.y, N.x, 0);
		float diameter = summonsList.front()->GetScale().x * 2;
		float combinedRadius = scale.x + target->GetScale().x;
		float offset = combinedRadius * 5;

		for (int i = 0; i < summonsList.size(); ++i)
		{
			if (!summonsList[i]->isDefending)
			{
				float wallLength = ((summonsList.size()) - 1) * diameter;
				wallLength = -wallLength * 0.5f;
				float formingWallLength = diameter * i;

				Vector3 offsetDir(target->pos.x - pos.x, target->pos.y - pos.y, 0);
				Vector3 destination = target->pos + offsetDir.Normalized() * offset;
				summonsList[i]->Goto(destination + (NP * (wallLength + formingWallLength)));
			}
		}
	}
}

void Summoner::UpdateCooldowns(double dt)
{
	if (summonCooldownTimer > 0)
	{
		summonCooldownTimer -= dt;
	}
	if (attackCooldownTimer > 0 && !attacking)
	{
		attackCooldownTimer -= dt;
	}
	if (attackCooldownTimer <= 0)
	{
		attacking = true;
	}
	if (attacking)
	{
		attackCooldownTimer += dt;
		if (attackCooldownTimer >= ATTACK_COOLDOWN)
		{
			attacking = false;
		}
	}
}

void Summoner::SetupMesh()
{
	float rotateAngle = Math::RadianToDegree(atan2f(this->pos.y - target->pos.y, this->pos.x - target->pos.x));

	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(rotateAngle - 90, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_SUMMONER];
}

Summons* FetchSummons()
{
	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
	{
		Summons *summons = dynamic_cast<Summons*>((*it));
		if (summons && summons->IsActive() == false)
		{
			summons->GameObject::SetType(GameObject::GO_ENTITY);
			summons->SetActive(true);
			return summons;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject::goList.push_back(new Summons());
	}
	Summons *summons = dynamic_cast<Summons*>(*(GameObject::goList.end() - 10));
	if (summons)
	{
		summons->GameObject::SetType(GameObject::GO_ENTITY);
		summons->SetActive(true);
		return summons;
	}

   { //for safety measure
	   Summons *summons = new Summons();
	   summons->GameObject::SetType(GameObject::GO_ENTITY);
	   summons->SetActive(true);
	   GameObject::goList.push_back(summons);
	   return summons;
   }
}