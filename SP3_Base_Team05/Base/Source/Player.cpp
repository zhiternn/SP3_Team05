#include "Player.h"
#include "Controls.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init(Vector3 pos, Vector3 scale, Vector3 front)
{
	this->pos.Set(pos.x, pos.y, pos.z);
	this->scale.Set(scale.x, scale.y, scale.z);
	this->front.Set(front.x, front.y, front.z);
	vel.SetZero();
	collider.type = Collider::COLLIDER_BALL;
}

void Player::UpdateInputs()
{
	if (Controls::GetInstance().OnHold(Controls::KEY_W))
	{
		// blah blah
	}
}

void Player::Update(double dt)
{
	GameObject::Update(dt);


}