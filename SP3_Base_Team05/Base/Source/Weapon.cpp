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
Weapon::Weapon() :w_ammo(0), w_dmgval(0)
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

void Weapon::Fire()
{
    switch (w_type)
    {
    case GUN:
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