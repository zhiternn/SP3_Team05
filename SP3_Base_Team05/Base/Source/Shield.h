/******************************************************************************/
/*!
\file	Shield.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class for Shield
*/
/******************************************************************************/
#ifndef SHIELD_H
#define SHIELD_h

#include "Weapon.h"

/******************************************************************************/
/*!
Class Shield.h:
\brief	Creates Shield for the game
*/
/******************************************************************************/
class Shield : public Weapon
{
public:
    Shield(Weapon::WEAPON_TYPE weaponType);     //< Overloaded Constructor
    ~Shield();                                  //< Destructor



private:
    WEAPON_TYPE shield;
    float CurrHealth;
    float MaxHealth;
};

#endif /* SHEILD_H */