#include "Summons.h"
#include "MeshManager.h"
#include "Player.h"
#include "Particle.h"

Summons::Summons()
{
}

Summons::~Summons()
{
	if (weapon)
		delete weapon;
}

void Summons::Init(Vector3 pos)
{
	Enemy::Init(pos);
	entityType = Entity::ENTITY_BOSS_BODY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	speedLimit = 100.f;
	scale.Set(7, 7, 7);
	health = 300;
	maxHealth = health;
	isDead = false;
	isDefending = false;
	randWeapon = Math::RandIntMinMax(1, 3);
	randProjectile = Math::RandIntMinMax(1, 2);
	switch (randWeapon)
	{
	case 1:
	{
		MachineGun *machinegun = new MachineGun(7);
		weapon = machinegun;
	}
	break;
	case 2:
	{
		Shotgun *shotgun = new Shotgun(5, 0.35f, 0.5f);
		weapon = shotgun;
	}
	break;
	case 3:
	{
		Splitgun *splitgun = new Splitgun(20, 5, 1);
		weapon = splitgun;
	}
	break;
	default:
		break;
	}
	switch (randProjectile)
	{
	case 1:
	{
		Bullet *bullet = new Bullet(5, 2.0f, 150.f, 3.f, 3);
		weapon->AssignProjectile(bullet);
	}
	break;
	case 2:
	{
		Hook *hook = new Hook(0, 0.1f, 20.0f);
		weapon->AssignProjectile(hook);
	}
	break;
	default:
		break;
	}
}

void Summons::Update(double dt)
{
	if (vel.LengthSquared() > speedLimit * speedLimit)
		vel = vel.Normalized() * speedLimit;

	GameObject::Update(dt);

	weapon->Update(dt);
}

void Summons::Die()
{
	isDead = true;
	active = false;
	if (weapon)
		delete weapon;
}

void Summons::HandleInteraction(GameObject* b, double dt)
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
				player->TakeDamage(SUMMON_TOUCH_DAMAGE);

			CollisionResponse(b);
		}
	}
}

void Summons::Goto(Vector3 pos)
{
	this->vel += (pos - this->pos).Normalized() * speedLimit;
	if (Reached(pos))
	{
		this->vel.SetZero();
	}
}

void Summons::Shoot(Vector3 dir)
{
	if (weapon != nullptr)
	{
		Vector3 playerPos = (dir - this->pos).Normalized();
		weapon->Fire(this->pos, playerPos, team);
	}
}

void Summons::SetupMesh()
{
	float rotateAngle = Math::RadianToDegree(atan2f(this->pos.y - target->pos.y, this->pos.x - target->pos.x));

	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(rotateAngle - 90, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_SUMMONS];
}