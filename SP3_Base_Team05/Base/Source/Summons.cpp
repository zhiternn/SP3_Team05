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
	health = 100;
	isDead = false;
	isDefending = false;
	weapon = new MachineGun();
	weapon->AssignProjectile(new Bullet());
}

void Summons::Update(double dt)
{
	GameObject::Update(dt);
	std::cout << this->captureRate << std::endl;
	weapon->Update(dt);

	if (vel.LengthSquared() > speedLimit * speedLimit)
		vel = vel.Normalized() * speedLimit;
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
	//float rotateAngle = Math::RadianToDegree(atan2f(this->pos.y - target->pos.y, this->pos.x - target->pos.x));

	modelStack.Translate(pos.x, pos.y, pos.z);
	//modelStack.Rotate(rotateAngle - 90, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);

	mesh = meshList[GEO_SUMMONS];
}