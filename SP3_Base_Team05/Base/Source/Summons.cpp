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
	isDefending = false;
	weapon = new MachineGun();
	weapon->AssignProjectile(new CProjectile);
}

void Summons::Update(double dt)
{
	GameObject::Update(dt);
	weapon->Update(dt);
	if (vel.LengthSquared() > speedLimit * speedLimit)
		vel = vel.Normalized() * speedLimit;
}

void Summons::Goto(Vector3 pos)
{
	this->vel += (pos - this->pos).Normalized() * movementSpeed;
}

void Summons::Shoot(Vector3 dir)
{
	Vector3 playerPos = (dir - this->pos).Normalized();
	weapon->Fire(this->pos, playerPos, team);
}