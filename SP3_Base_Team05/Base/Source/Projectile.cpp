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

/******************************************************************************/
/*!
\brief	CProjectile Default Constructor
*/
/******************************************************************************/
CProjectile::CProjectile():
GameObject(GameObject::GO_PROJECTILE),
proj_team(PROJECTILE_TEAM::TEAM_NEUTRAL)
{
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

/******************************************************************************/
/*!
\brief
GetProjectileType to get the projectile type

\return
Projectile Type
*/
/******************************************************************************/
CProjectile::PROJECTILE_TEAM CProjectile::GetTeam()
{
    return proj_team;
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

/******************************************************************************/
/*!
\brief
Setting the Projectile Type with a new type;

\param type
The new type to replace the old one
*/
/******************************************************************************/
void CProjectile::SetTeam(PROJECTILE_TEAM team)
{
	this->proj_team = team;
}

void CProjectile::Init(Vector3 pos, Vector3 dir, float speed)
{
	this->active = true;
	this->type = GameObject::GO_PROJECTILE;
	this->collider.type = Collider::COLLIDER_BALL;
	this->pos = pos;
	this->vel = dir.Normalized() * speed;
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
		active = false;
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
    if (b->GetType() == GameObject::GO_PROJECTILE)
        return;
	
	if (this->proj_team == PROJECTILE_TEAM::TEAM_PLAYER && b->GetType() == GameObject::GO_ENEMY)
	{
		if (CheckCollision(b, dt))
		{
			CollisionResponse(b);
			this->active = false;
		}
	}
	else if (this->proj_team == PROJECTILE_TEAM::TEAM_ENEMY && b->GetType() == GameObject::GO_PLAYER)
	{
		if (CheckCollision(b, dt))
		{
			CollisionResponse(b);
			this->active = false;
		}
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