/******************************************************************************/
/*!
\file	Weapon.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Generic Class for Weapons
*/
/******************************************************************************/
#ifndef WEAPON_H
#define WEAPON_H
/******************************************************************************/
/*!
Class Weapon.h:
\brief	Creates Weapon for the game
*/
/******************************************************************************/
class Weapon
{
public:
    Weapon();
    ~Weapon();
    enum WEAPON_TYPE
    {
        GUN,
        ROPEGUN,
        TRAP,
        SHIELD,
        GRENADE,
        WEAPON_TOTAL
    };

    void Fire();            //< Weapon Fire

    WEAPON_TYPE w_type;     //< Weapon Type
    float w_ammo;           //< Weapon Ammo Amount 
    float w_dmgval;         //< Weapon Damage Value

};


#endif /* WEAPON_H */
