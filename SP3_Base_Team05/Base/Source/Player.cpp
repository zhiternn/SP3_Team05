#include "Player.h"
#include "Controls.h"
#include "Projectile.h"

Player::Player():
weapon(NULL)
{
	inventory = new Inventory();
	inventory->weapons.front()->AssignProjectile(inventory->bullets.front());
	weapon = inventory->weapons.front();
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
	team = TEAM_PLAYER;
	active = true;
	type = GameObject::GO_ENTITY;
	collider.type = Collider::COLLIDER_BALL;
	mass = 1;
	vel.SetZero();
	isDashed = false;
}

void Player::Update(double dt)
{
	GameObject::Update(dt);
	weapon->Update(dt);
	isMoving = false;
	if (!isMoving)
	{
		vel *= 0.9f;
	}
	if (isDashed)
	{
		cooldownTimer -= dt;
		if (cooldownTimer <= 0)
		{
			isDashed = false;
		}
	}
	if (health <= 0)
	{
		active = false;
	}
}

void Player::HandleInteraction(GameObject* b, double dt)
{
	if (b->GetType() == GameObject::GO_ENVIRONMENT)
	{
		GameObject::HandleInteraction(b, dt);
	}
}

void Player::Move(Vector3 dir, double dt)
{
	if (vel.LengthSquared() < (MOVEMENT_LIMIT)* (MOVEMENT_LIMIT))
	{
		isMoving = true;
		forceMagnitude = MOVEMENT_LIMIT;
		this->ApplyForce(dt, dir, forceMagnitude);
	}
}

void Player::Dash(Vector3 dir, double dt)
{
	if (!isDashed && !dir.IsZero())
	{
		forceMagnitude = MOVEMENT_LIMIT * DASH_DISTANCE;
		this->ApplyForce(dt, dir, forceMagnitude);
		isDashed = true;
		cooldownTimer = DASH_COOLDOWN;
	}
}

void Player::Shoot(Vector3 dir)
{
	this->weapon->Fire(this->pos, dir, CProjectile::TEAM_PLAYER);
}

void Player::Shield(Vector3 dir)
{
    //weapon->Fire();
    Vector3 tempPos(pos.x + dir.x * (scale.x + 4), pos.y + dir.y * (scale.y + 4), 0);
    this->weapon->Fire(tempPos, dir, CProjectile::TEAM_PLAYER);
}

void Player::ChangeWeaponUp()
{
	weaponIter++;
	if (weaponIter >= inventory->weapons.size())
	{
		weaponIter = 0;
	}
	weapon = inventory->weapons[weaponIter];
	weapon->AssignProjectile(inventory->bullets[projectileIter]);
}

void Player::ChangeWeaponDown()
{
	if (weaponIter <= 0)
	{
		weaponIter = inventory->weapons.size() - 1;
	}
	else
	{ 
		weaponIter--;
	}
	weapon = inventory->weapons[weaponIter];
	weapon->AssignProjectile(inventory->bullets[projectileIter]);
}

void Player::ChangeProjectileUp()
{
	projectileIter++;
	if (projectileIter >= inventory->bullets.size())
	{
		projectileIter = 0;
	}
	projectile = inventory->bullets[projectileIter];
	weapon->AssignProjectile(projectile);
}
void Player::ChangeProjectileDown()
{
	if (projectileIter <= 0)
	{
		projectileIter = inventory->bullets.size() - 1;
	}
	else
	{
		projectileIter--;
	}
	projectile = inventory->bullets[projectileIter];
	weapon->AssignProjectile(projectile);
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

void Player::SetupMesh()
{
	mesh = NULL;
}