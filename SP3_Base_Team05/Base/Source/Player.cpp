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
}

void Player::UpdateInputs(double dt)
{
	float forceMagnitude = MOVEMENT_SPEED;
	Vector3 forceDir;
	if (Controls::GetInstance().OnPress(Controls::KEY_SPACE))
	{
		forceMagnitude = 1000;
	}
	if (Controls::GetInstance().OnHold(Controls::KEY_W))
	{
		forceDir.y += 1;
	} 
	if (Controls::GetInstance().OnHold(Controls::KEY_S))
	{
		forceDir.y -= 1;
	}
	if (Controls::GetInstance().OnHold(Controls::KEY_A))
	{
		forceDir.x -= 1;
	}
	if (Controls::GetInstance().OnHold(Controls::KEY_D))
	{
		forceDir.x += 1;
	}

	if (!forceDir.IsZero())
	{
		forceDir.Normalize();
		this->ApplyForce(dt, forceDir, forceMagnitude);
	}

	//if (vel.LengthSquared() > (MOVEMENT_LIMIT) * (MOVEMENT_LIMIT))
	//{
	//	vel.Normalize();
	//	vel *= MOVEMENT_LIMIT;
	//}
}

void Player::Update(double dt)
{
	GameObject::Update(dt);
}