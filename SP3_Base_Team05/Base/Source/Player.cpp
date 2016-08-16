#include "Player.h"
#include "Controls.h"
#include "Projectile.h"

Player::Player():
GameObject(GameObject::GO_BALL)
{
}

Player::~Player()
{
}

void Player::Init(Vector3 pos, Vector3 scale, Vector3 front)
{
	weapon = new Weapon();
	this->scale.Set(scale.x, scale.y, scale.z);
	this->pos.Set(pos.x, pos.y, pos.z);
	this->front.Set(front.x, front.y, front.z);
	active = true;
	type = GameObject::GO_BALL;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	vel.SetZero();
	isDashed = false;
}

void Player::Update(double dt)
{
	GameObject::Update(dt);
	isMoving = false;
	if (!isMoving)
	{
		vel *= 0.9f;
	}
	if (isDashed)
	{
		cooldownTimer -= 1.f * dt;
		if (cooldownTimer <= 0)
		{
			isDashed = false;
		}
	}
}

void Player::Move(Vector3 dir, double dt)
{
	if (vel.LengthSquared() < (MOVEMENT_LIMIT)* (MOVEMENT_LIMIT))
	{
		isMoving = true;
		forceMagnitude = MOVEMENT_SPEED;
		this->ApplyForce(dt, dir, forceMagnitude);
	}
}

void Player::Dash(Vector3 dir, double dt)
{
	if (!isDashed && !dir.IsZero())
	{
		forceMagnitude = MOVEMENT_SPEED * 100.0f;
		this->ApplyForce(dt, dir, forceMagnitude);
		isDashed = true;
		cooldownTimer = DASH_COOLDOWN;
	}
}

void Player::Shoot()
{
	weapon->Fire();
}

void Player::SetMoving(bool isMoving)
{
	this->isMoving = isMoving;
}

void Player::SetDashed(bool isDashed)
{
	this->isDashed = isDashed;
}

bool Player::IsMoving()
{
	return isMoving;
}

bool Player::IsDashed()
{
	return isDashed;
}
