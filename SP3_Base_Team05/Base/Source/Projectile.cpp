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
CProjectile::CProjectile()
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
GetProjectileState to get the projectile count

\return
Projectile State
*/
/******************************************************************************/
bool CProjectile::GetState()
{
    return proj_active;
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
PROJECTILE_TYPE CProjectile::GetType()
{
    return proj_type;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile State with a new state;

\param state
The new state to replace the old one
*/
/******************************************************************************/
void CProjectile::SetState(bool state)
{
    proj_active = state;
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
    proj_type = type;
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
    //GameObject::HandleInteraction(proj_type, dt);
	proj_lifetime -= dt;
}

/******************************************************************************/
/*!
\brief
Handling the Interaction with another Game Object

\param dt
Time active
*/
/******************************************************************************/
void CProjectile::HandleInteraction(CProjectile* b, double dt)
{
    if (b->type == GameObject::GO_PROJECTILE)
        return;

    GameObject::HandleInteraction(b, dt);
    this->active = false;
}

CProjectile* FetchProjectile()
{
   std::vector<GameObject*>::iterator it;
   for (it = GameObject::goList.begin(); it != GameObject::goList.end(); ++it)
   {
       CProjectile *proj = dynamic_cast<CProjectile*>((*it));
       if (proj && proj->GetActive() == false)
       {
           proj->SetActive(true);
           return proj;
       }
   }

   for (int i = 0; i < 10; ++i)
   {
       GameObject::goList.push_back(new CProjectile());
   }
   CProjectile *proj = dynamic_cast<CProjectile*>(GameObject::goList.back() - 10);
   if (proj)
   {
       proj->SetActive(true);
       return proj;
   }
    
   { //for safety measure
       CProjectile *proj = new CProjectile();
       proj->SetActive(true);
       GameObject::goList.push_back(proj);
       return proj;
   }
}