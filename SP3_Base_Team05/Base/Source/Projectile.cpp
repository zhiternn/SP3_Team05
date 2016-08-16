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
GameObject(GameObject::GO_PROJECTILE)
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

/******************************************************************************/
/*!
\brief
Setting the Projectile Type with a new type;

\param type
The new type to replace the old one
*/
/******************************************************************************/
void CProjectile::SetType(PROJECTILE_TYPE type)
{
	switch (type)
	{
	case CProjectile::P_PLAYER_BULLET:
		proj_dmg = 10;
		proj_lifetime = 5;
		proj_type = type;
		break;
	case CProjectile::P_PLAYER_GRENADE:
		proj_dmg = 50;
		proj_lifetime = 3;
		proj_type = type;
		break;
	case CProjectile::P_PLAYER_ROPE:
		proj_dmg = 10;
		proj_lifetime = 5;
		proj_type = type;
		break;
	case CProjectile::P_PLAYER_SHIELD:
		proj_dmg = 10;
		proj_lifetime = 5;
		proj_type = type;
		break;
	case CProjectile::P_PLAYER_TRAP:
		proj_dmg = 10;
		proj_lifetime = 5;
		proj_type = type;
		break;
	default:
		break;
	}
}

void CProjectile::Init(Vector3 pos, Vector3 dir, float speed)
{
	std::cout << "hehe" << std::endl;
	this->active = true;
	this->type = GameObject::GO_PROJECTILE;
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

	if (CheckCollision(b, dt))
		this->active = false;
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