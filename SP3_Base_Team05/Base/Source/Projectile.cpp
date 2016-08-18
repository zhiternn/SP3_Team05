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

CProjectile::PROJECTILE_TYPE CProjectile::GetType()
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

void CProjectile::SetType(PROJECTILE_TYPE type)
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
		if (this->proj_type == (CProjectile::TRAP))
		{
			for (int i = 0; i < GameObject::goList.size(); i++)
			{
				if (GameObject::goList[i]->GetType() == CProjectile::TRAP)
				{
					GameObject::goList.erase(GameObject::goList.begin() + i);
				}
			}
		}
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
		}
		this->active = false;
	}
}

CProjectile* FetchProjectile()
{
   std::vector<GameObject*>::iterator it;
   for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
   {
       CProjectile *proj = dynamic_cast<CProjectile*>((*it));
	   if (proj && proj->IsActive() == false)
       {
			proj->GameObject::SetType(GameObject::GO_PROJECTILE);
			proj->SetActive(true);
			return proj;
       }
   }

   for (int i = 0; i < 10; ++i)
   {
       GameObject::goList.push_back(new CProjectile());
   }
   CProjectile *proj = dynamic_cast<CProjectile*>(*(GameObject::goList.end() - 10));
   if (proj)
   {
	   proj->GameObject::SetType(GameObject::GO_PROJECTILE);
       proj->SetActive(true);
       return proj;
   }
    
   { //for safety measure
       CProjectile *proj = new CProjectile();
	   proj->GameObject::SetType(GameObject::GO_PROJECTILE);
       proj->SetActive(true);
       GameObject::goList.push_back(proj);
       return proj;
   }
}
