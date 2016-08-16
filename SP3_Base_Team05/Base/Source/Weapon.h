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

#include "Vector3.h"

/******************************************************************************/
/*!
Class Weapon.h:
\brief	Creates Weapon for the game.
*/
/******************************************************************************/
class Weapon
{
public:
    Weapon();                                   //< 
    virtual ~Weapon();                          //<
    virtual void Update(double dt);             //<


    enum WEAPON_TYPE
    {
        GUN,
        ROPEGUN,
        TRAP,
        SHIELD,
        GRENADE,
        WEAPON_TOTAL,
    };

    void Fire(Vector3 pos, Vector3 dir);        //< Weapon Fire
    
    // Getters
    WEAPON_TYPE GetWeaponType();                //< Returns Weapon Type
    float GetWeaponAmmo();                      //< Returns Weapon Ammo Amount
    float GetDMGVal();                          //< Returns Get Damage Value
    float GetProjSpd();                         //< Returns Weapon Projectile Speed
    float GetProjLifetime();                    //< Returns Projectile Lifetime

    // Setters
    void SetWeaponType(WEAPON_TYPE type);       //< Sets Weapon Type
    void SetWeaponAmmo(float ammo);             //< Sets Weapon Ammo Amount
    void SetDMGVal(float dmg);                  //< Sets Get Damage Value
    void SetProjSpd(float speed);               //< Sets Weapon Projectile Speed
    void SetProjLifetime(float lifetime);       //< Sets Projectile Lifetime

    WEAPON_TYPE w_type;                         //< Weapon Type
    float w_ammo;                               //< Weapon Ammo Amount 
    float w_dmgval;                             //< Weapon Damange Value
    float w_projectileSpeed;                    //< Weapon Projectile Speed
    float w_projectileLifetime;                 //< Weapon Projectile Lifetime

};


#endif /* WEAPON_H */
