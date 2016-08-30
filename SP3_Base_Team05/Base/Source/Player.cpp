#include "Player.h"
#include "Controls.h"
#include "Projectile.h"

Player::Player():
weapon(NULL)
{
	inventory = new Inventory();
	inventory->weapons.front()->AssignProjectile(inventory->bullets.front());
	weapon = inventory->weapons.front();
	projectile = inventory->bullets.front();
	currency = 0;

    shield = new Shield();
    GameObject::goList.push_back(shield);
    this->shield->SetActive(true);
    this->shield->SetAliveState(true);
    this->shield->SetColliderType(Collider::COLLIDER_BOX);
    this->shield->SetScale(4, 4, 4);
    this->shield->SetMass(3);
    this->shield->SetVelocity(0, 0, 0);
}

Player::~Player()
{
	//if (weapon)
	//	delete weapon;
}

void Player::Init(Vector3 pos)
{
	damageBuffer = DAMAGE_BUFFER;
	this->pos.Set(pos.x, pos.y, pos.z);

	team = TEAM_PLAYER;
	active = true;
	type = GameObject::GO_ENTITY;
	entityType = Entity::ENTITY_PLAYER;
	collider.type = Collider::COLLIDER_BALL;
	health = 100;
	maxHealth = health;
	mass = 1;
	vel.SetZero();
	isDashed = false;
	isDead = false;
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

	if (damageBuffer > 0)
		damageBuffer -= dt;
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
		this->ApplyForce(dir, forceMagnitude * dt);
	}
}

void Player::Dash(Vector3 dir, double dt)
{
	if (!isDashed && !dir.IsZero())
	{
		forceMagnitude = MOVEMENT_LIMIT * DASH_DISTANCE;
		this->ApplyForce(dir, forceMagnitude * dt);
		isDashed = true;
		cooldownTimer = DASH_COOLDOWN;
	}
}

void Player::Shoot(Vector3 dir)
{
	this->weapon->Fire(this->pos, dir, CProjectile::TEAM_PLAYER);
}

void Player::Shielding(Vector3 dir)
{
    //weapon->Fire();
    Vector3 tempPos(pos.x + dir.x * (scale.x + 4), pos.y + dir.y * (scale.y + 4), 0);

    this->shield->SetActive(true);
    this->shield->SetAliveState(true);
    this->shield->SetColliderType(Collider::COLLIDER_BOX);
    this->shield->SetScale(4, 4, 4);
    this->shield->SetMass(3);

    this->shield->Init(tempPos);
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

void Player::TakeDamage(int amount)
{
	if (damageBuffer <= 0.0f)
	{
		damageBuffer = DAMAGE_BUFFER;
		Entity::TakeDamage(amount);
	}
}
