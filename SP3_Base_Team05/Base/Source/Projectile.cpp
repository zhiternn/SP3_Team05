/******************************************************************************/
/*!
\file	Projectile.cpp
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class to create Projectiles
*/
/******************************************************************************/
#include "Projectile.h"
#include "Entity.h"
#include "ProjectileList.h"

/******************************************************************************/
/*!
\brief	CProjectile Default Constructor
*/
/******************************************************************************/

CProjectile::CProjectile(PROJECTILE_TYPE type) :
GameObject(GameObject::GO_PROJECTILE),
proj_type(type)
{
	proj_dmg = 10;
	proj_lifetime = 2;
	proj_speed = 100;
}

/******************************************************************************/
/*!
\brief	Vector3 destructor
*/
/******************************************************************************/
CProjectile::~CProjectile()
{
}

/******************************************************************************/
/*!
\brief
GetProjectileDMG to get the projectile damage

\return
Projectile Damage
*/
/******************************************************************************/
float CProjectile::GetDMG()
{
    return proj_dmg;
}

/******************************************************************************/
/*!
\brief
GetProjectileLifetime to get the projectile lifetime

\return
Projectile Lifetime
*/
/******************************************************************************/
float CProjectile::GetLifetime()
{
    return proj_lifetime;
}

float CProjectile::GetProjectileSpeed()
{
	return proj_speed;
}

CProjectile::PROJECTILE_TYPE CProjectile::GetProjType()
{
	return proj_type;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Damage with a new damage;

\param damage
The new damage to replace the old one
*/
/******************************************************************************/
void CProjectile::SetDMG(float damage)
{
    proj_dmg = damage;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Lifetime with a new lifetime;

\param lifetime
The new lifetime to replace the old one
*/
/******************************************************************************/
void CProjectile::SetLifetime(float lifetime)
{
    proj_lifetime = lifetime;
}

void CProjectile::SetProjectileSpeed(float speed)
{
	this->proj_speed = speed;
}

void CProjectile::SetProjType(PROJECTILE_TYPE type)
{
	this->proj_type = type;
}

void CProjectile::Init(Vector3 pos, Vector3 dir)
{
	this->active = true;
	this->type = GameObject::GO_PROJECTILE;
	this->collider.type = Collider::COLLIDER_BALL;
	this->pos = pos;
	this->vel = dir.Normalized() * proj_speed;
}

/******************************************************************************/
/*!
\brief
Update the Game Object and Projectile

\param dt
Time active
*/
/******************************************************************************/
void CProjectile::Update(double dt)
{
	GameObject::Update(dt);
	proj_lifetime -= dt;
	if (proj_lifetime <= 0)
	{
		active = false;
	}
}

/******************************************************************************/
/*!
\brief
Handling the Interaction with another Game Object

\param dt
Time active
*/
/******************************************************************************/
void CProjectile::HandleInteraction(GameObject* b, double dt)
{
	if (CheckCollision(b, dt))
	{
		Entity* entity = dynamic_cast<Entity*>(b);
		if (entity)
		{
			entity->TakeDamage(proj_dmg);
			CollisionResponse(b);
		}
		this->active = false;
	}
}

void CProjectile::HandleOutOfBounds(float minX, float maxX, float minY, float maxY)
{
	if (this->pos.x < minX || this->pos.x > maxX ||
		this->pos.y < minY || this->pos.y > maxY)
	{
		this->active = false;
	}

}