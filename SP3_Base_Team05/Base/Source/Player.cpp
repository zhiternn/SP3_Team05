#include "Player.h"
#include "Controls.h"
#include "Projectile.h"

Player::Player():
weapon(NULL),
GameObject(GameObject::GO_PLAYER)
{
}

Player::~Player()
{
	//if (weapon)
	//	delete weapon;
}

void Player::Init(Vector3 pos, Vector3 scale, Vector3 front)
{
	this->scale.Set(scale.x, scale.y, scale.z);
	this->pos.Set(pos.x, pos.y, pos.z);
	this->front.Set(front.x, front.y, front.z);
	active = true;
	type = GameObject::GO_PLAYER;
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

void Player::Shoot(Vector3 dir)
{
	this->weapon->Fire(this->pos, dir, CProjectile::TEAM_PLAYER);
}

void Player::ChangeWeapon(int type)
{
	//switch (type)
	//{
	//case 1:
	//	weapon->SetWeaponType(Weapon::GUN);
	//	break;
	//case 2:
	//	weapon->SetWeaponType(Weapon::ROPEGUN);
	//	break;
	//case 3:
	//	weapon->SetWeaponType(Weapon::TRAP);
	//	break;
	//case 4:
	//	weapon->SetWeaponType(Weapon::SHIELD);
	//	break;
	//case 5:
	//	weapon->SetWeaponType(Weapon::GRENADE);
	//	break;
	//default:
	//	break;
	//}

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



