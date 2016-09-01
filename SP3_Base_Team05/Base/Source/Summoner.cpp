#include "Summoner.h"
#include "Controls.h"
#include "MeshManager.h"
#include "Player.h"
#include "Particle.h"
#include "SceneBase.h"

Summoner::Summoner():
Enemy()
{
}

Summoner::~Summoner()
{
	for (auto q : summonsList)
	{
		if (q)
		{
			delete q;
		}
	}
}

void Summoner::Init(Vector3 pos)
{
	Enemy::Init(pos);
	collider.type = Collider::COLLIDER_BALL;
	Entity::entityType = Entity::ENTITY_BOSS_MAIN;
	active = true;
	mass = 10;
	speedLimit = 60.0f;
	movementSpeed = 60.0f;
	scale.Set(15, 15, 15);
	health = 1000;
	maxHealth = health;
	isDead = false;
	attacking = false;
	teleported = false;
	agressiveLevel = 1 - ((float)health / maxHealth);
	safetyThreshold = this->GetScale().x * 6;
	chaseThreshold = safetyThreshold * 1.6f;
	for (auto q : summonsList)
	{
		q->Init(this->pos);
		q->SetTarget(target);
	}
}

void Summoner::Update(double dt)
{
	Enemy::Update(dt);
	UpdateCooldowns(dt);
	// if there's no summons on the field, instantly summon all
	if (summonsList.size() <= 0 && summonCooldownTimer <= 0)
	{
		summonCooldownTimer = SUMMONING_COOLDOWN;
		SummonAll();
	}
	// if there are summons on the field, summon one by one
	if (summonsList.size() < AMOUNT_OF_SUMMONS && summonCooldownTimer <= 0)
	{
		summonCooldownTimer = SUMMONING_COOLDOWN;
		Summons* summons = FetchSummons();
		summons->Init(this->pos);
		summons->SetTarget(target);
		summonsList.push_back(summons);
	}
	if (this->health < maxHealth)
	{
		static float healthregenCooldown = 0;
		healthregenCooldown += dt;
		if (healthregenCooldown >= 1)
		{
			healthregenCooldown = 0.0f;
			if (health + SUMMONER_HEALTH_REGEN_PERSEC > maxHealth)
				health = maxHealth;
			else
				health += SUMMONER_HEALTH_REGEN_PERSEC;
		}
	}
	CleaningUpMess();
	Defend();
	Attack();
	for (auto q : summonsList)
	{
		if (!q->isDefending && attacking)
		{
			q->Shoot(target->pos);
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
			vel = vel * 0.9f;
		}
	}
	if (teleported)
	{
		this->vel.SetZero();
	}
}

void Summoner::HandleInteraction(GameObject* b, double dt)
{
	if (b->GetType() == GameObject::GO_WALL)
		return;

	Enemy::HandleInteraction(b, dt);
	if (b->GetType() == GameObject::GO_ENTITY)
	{
		if (CheckCollision(b, dt))
		{
			Player* player = dynamic_cast<Player*>(b);
			if (player)
				player->TakeDamage(SUMMONER_TOUCH_DAMAGE);

			CollisionResponse(b);
		}
	}
}

void Summoner::HandleOutOfBounds(float minX, float maxX, float minY, float maxY)
{
	Vector3 randPos(
		Math::RandFloatMinMax(minX, maxX), 
		Math::RandFloatMinMax(minY, maxY), 
		0);
	//Checks against minX
	if ((this->GetPosition().x - this->GetScale().x < minX && this->GetVelocity().x < 0) ||
		(this->GetPosition().x + this->GetScale().x > maxX && this->GetVelocity().x > 0) ||
		(this->GetPosition().y - this->GetScale().y < minY && this->GetVelocity().y < 0) ||
		(this->GetPosition().y + this->GetScale().y > maxY && this->GetVelocity().y > 0))
	{
		EmitTeleportParticle(this->pos, this->scale.x);
		this->pos = randPos;
		teleported = true;
	}
}

void Summoner::TakeDamage(unsigned amount)
{
	Entity::TakeDamage(amount);
	agressiveLevel = 1 - ((float)health / maxHealth);
	if (teleported)
	{
		teleported = false;
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

void Summoner::SummonAll()
{
	for (int i = 0; i < AMOUNT_OF_SUMMONS; ++i)
	{
		Summons* summons = FetchSummons();
		summons->Init(pos);
		summons->SetTarget(target);
		summons->SetTeam(this->team);
		summonsList.push_back(summons);
	}
}

void Summoner::Defend()
{
	
	if (!summonsList.empty())
	{
		Vector3 N = (target->pos - this->pos).Normalized();
		Vector3 NP = Vector3(-N.y, N.x, 0);
		float diameter = summonsList.front()->GetScale().x;
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
	if (!summonsList.empty())
	{
		Vector3 N = (target->pos - this->pos).Normalized();
		Vector3 NP = Vector3(-N.y, N.x, 0);
		float diameter = summonsList.front()->GetScale().x * 2;
		float combinedRadius = scale.x + target->GetScale().x;
		float offset = combinedRadius * 3;

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
		EmitAttackParticle(this->pos, this->scale.x * 5, true);
		attacking = true;
		for (auto q : summonsList)
		{
			if (q->IsActive())
			{
				EmitAttackParticle(q->pos, q->GetScale().x * 7, false);
			}
		}
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