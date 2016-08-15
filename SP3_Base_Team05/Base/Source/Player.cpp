#include "Player.h"
#include "Controls.h"

Player::Player():
GameObject(GameObject::GO_BALL)
{
}

Player::~Player()
{
}

void Player::Init(Vector3 pos, Vector3 scale, Vector3 front)
{
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

void Player::UpdateInputs(double dt)
{
	isMoving = false;
	float forceMagnitude = 0;
	Vector3 forceDir;

	if (Controls::GetInstance().OnHold(Controls::KEY_W))
	{
		forceDir.y += 1;
		if (vel.LengthSquared() < (MOVEMENT_LIMIT)* (MOVEMENT_LIMIT))
		{
			isMoving = true;
			forceMagnitude = MOVEMENT_SPEED;
		}
	}
	if (Controls::GetInstance().OnHold(Controls::KEY_S))
	{
		forceDir.y -= 1;
		if (vel.LengthSquared() < (MOVEMENT_LIMIT)* (MOVEMENT_LIMIT))
		{
			isMoving = true;
			forceMagnitude = MOVEMENT_SPEED;
		}
	}
	if (Controls::GetInstance().OnHold(Controls::KEY_A))
	{
		forceDir.x -= 1;
		if (vel.LengthSquared() < (MOVEMENT_LIMIT)* (MOVEMENT_LIMIT))
		{
			isMoving = true;
			forceMagnitude = MOVEMENT_SPEED;
		}
	}
	if (Controls::GetInstance().OnHold(Controls::KEY_D))
	{
		forceDir.x += 1;
		if (vel.LengthSquared() < (MOVEMENT_LIMIT)* (MOVEMENT_LIMIT))
		{
			isMoving = true;
			forceMagnitude = MOVEMENT_SPEED;
		}
	}

	if (Controls::GetInstance().OnPress(Controls::KEY_SPACE) && !isDashed)
	{
		forceMagnitude = MOVEMENT_SPEED * 100.0f;
		isDashed = true;
		cooldownTimer = DASH_COOLDOWN;
	}
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
	if (!forceDir.IsZero())
	{
		forceDir.Normalize();
		this->ApplyForce(dt, forceDir, forceMagnitude);
	}
}

void Player::Update(double dt)
{
	GameObject::Update(dt);
}
