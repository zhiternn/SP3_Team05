/******************************************************************************/
/*!
\file	Weapon.cpp
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Generic Class for Weapons
*/
/******************************************************************************/

#include "Weapon.h"
#include "Projectile.h"

/******************************************************************************/
/*!
\brief	Weapon Default Constructor
*/
/******************************************************************************/
Weapon::Weapon() : 
w_ammo(0),
w_dmgval(0)
{

}

/******************************************************************************/
/*!
\brief	Weapon destructor
*/
/******************************************************************************/
Weapon::~Weapon()
{

}

/******************************************************************************/
/*!
\brief
Updates Weapon Class

\return
None
*/
/******************************************************************************/
void Weapon::Update(double dt)
{
}

/******************************************************************************/
/*!
\brief
Get Weapon Type to get the Weapon Type

\return
Weapon Type
*/
/******************************************************************************/
Weapon::WEAPON_TYPE Weapon::GetWeaponType()
{
    return w_type;
}

/******************************************************************************/
/*!
\brief
Get Weapon Ammo to get the Ammo Amount

\return
Weapon Ammo Amount
*/
/******************************************************************************/
float Weapon::GetWeaponAmmo()
{
    return w_ammo;
}

/******************************************************************************/
/*!
\brief
Get Damage Value of the Weapon

\return
Damage Value
*/
/******************************************************************************/
float Weapon::GetDMGVal()
{
    return w_dmgval;
}

/******************************************************************************/
/*!
\brief
Get Projectile Speed of the Weapon

\return
Projectile Speed
*/
/******************************************************************************/
float Weapon::GetProjSpd()
{
    return w_projectileSpeed;
}

/******************************************************************************/
/*!
\brief
Get Projectile Lifetime of the Weapon

\return
Projectile Lifetime
*/
/******************************************************************************/
float Weapon::GetProjLifetime()
{
    return w_projectileLifetime;
}

/******************************************************************************/
/*!
\brief
Setting the Weapon Type with a new type;

\param type
The new type to replace the old one
*/
/******************************************************************************/
void Weapon::SetWeaponType(WEAPON_TYPE type)
{
    w_type = type;
}

/******************************************************************************/
/*!
\brief
Setting the Ammo Amount with a new amount;

\param ammo
The new ammo amount to replace the old one
*/
/******************************************************************************/
void Weapon::SetWeaponAmmo(float ammo)
{
    w_ammo = ammo;
}

/******************************************************************************/
/*!
\brief
Setting the Damage Amount with a new amount;

\param dmg
The new dmg to replace the old one
*/
/******************************************************************************/
void Weapon::SetDMGVal(float dmg)
{
    w_dmgval = dmg;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Speed with a new amount;

\param dmg
The new speed to replace the old one
*/
/******************************************************************************/
void Weapon::SetProjSpd(float speed)
{
    w_projectileSpeed = speed;
}

/******************************************************************************/
/*!
\brief
Setting the Projectile Lifetime with a new amount;

\param dmg
The new lifetime to replace the old one
*/
/******************************************************************************/
void Weapon::SetProjLifetime(float lifetime)
{
    w_projectileLifetime = lifetime;
}

void Weapon::Fire(Vector3 pos, Vector3 dir)
{
    switch (w_type)
    {
    case GUN:
    {
        CProjectile* bullet = FetchProjectile();
        bullet->SetLifetime(w_projectileLifetime);
        bullet->SetPostion(pos);
        dir = dir.Normalized() * w_projectileSpeed;
        bullet->SetVelocity(dir);
        bullet->SetScale(1, 1, 1);
        bullet->SetMass(1);
        bullet->SetColliderType(Collider::COLLIDER_BALL);
    }
        break;
    case ROPEGUN:
        break;
    case SHIELD:
        break;
    case GRENADE:
        break;
    default:
        break;
    }
}