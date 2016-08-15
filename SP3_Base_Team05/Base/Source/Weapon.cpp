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

/******************************************************************************/
/*!
\brief	Weapon Default Constructor
*/
/******************************************************************************/
Weapon::Weapon() : w_active(false), w_ammo(0), w_dmgval(0)
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

void Weapon::Fire()
{
    switch (w_type)
    {
    case GUN:
        projectile.SetType(P_PLAYER_BULLET);
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