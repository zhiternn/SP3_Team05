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

#include "Projectile.h"

/******************************************************************************/
/*!
Class Shield.h:
\brief	Creates Shield for the game
*/
/******************************************************************************/
class Shield : public CProjectile
{
public:
    Shield(CProjectile::PROJECTILE_TYPE projectileType);                //< Overloaded Constructor
    ~Shield();                                                          //< Destructor

    virtual void Update(double dt);                                     //< Update

    void regenerateShield(float currHP, double dt);                     //< Regenerates Shield Health

    // Getter
    bool GetActiveState();                                              //< Sets Shield Active State
    bool GetAliveState();                                               //< Sets Shield Alive State
    float GetCurrHealth();                                              //< Sets Shield Curr Health
    float GetMaxHealth();                                               //< Sets Shield Max Health

    // Setter
    void SetActiveState(bool active);                                   //< Returns Shield Active State 
    void SetAliveState(bool alive);                                     //< Returns Shield Alive State
    void SetCurrHealth(float curr);                                     //< Returns Shield Curr Health
    void SetMaxHealth(float max);                                       //< Returns Shield Max Health

private:
    bool activeState;
    bool aliveState;
    float CurrHealth;
    float MaxHealth;
    float startShieldRegen;
};

#endif /* SHEILD_H */