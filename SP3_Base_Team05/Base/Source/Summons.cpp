#include "Summons.h"
#include "MeshManager.h"

Summons::Summons()
{
}

Summons::~Summons()
{
}

void Summons::Init(Vector3 pos)
{
	Enemy::Init(pos);
	entityType = Entity::ENTITY_BOSS_BODY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	speedLimit = 50.f;
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
		weapon = new MachineGun();
		break;
	case 2:
		weapon = new Shotgun();
		break;
	case 3:
		weapon = new Splitgun();
		break;
	default:
		break;
	}
	switch (randProjectile)
	{
	case 1:
		weapon->AssignProjectile(new Bullet);
		break;
	case 2:
		weapon->AssignProjectile(new Hook);
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
	Vector3 playerPos = (dir - this->pos).Normalized();
	weapon->Fire(this->pos, playerPos, team);
}

void Summons::SetupMesh()
{
	float rotateAngle = Math::RadianToDegree(atan2f(this->pos.y - target->pos.y, this->pos.x - target->pos.x));

	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(rotateAngle - 90, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_SUMMONS];
}