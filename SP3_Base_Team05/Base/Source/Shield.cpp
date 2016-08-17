/******************************************************************************/
/*!
\file	Shield.cpp
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Class for Shield
*/
/******************************************************************************/

#include "Shield.h"

/******************************************************************************/
/*!
\brief	Shield Default Constructor
*/
/******************************************************************************/
Shield::Shield(CProjectile::PROJECTILE_TYPE projectileType) :
CurrHealth(0),
MaxHealth(2000),
activeState(false),
aliveState(false)
{

}

void Shield::Update(double dt)
{
    CProjectile::Update(dt);
    if (activeState == false)
        SetLifetime(0);

    if (CurrHealth <= 0)
    {
        SetLifetime(0);
    }

    regenerateShield(CurrHealth, dt);
}

void Shield::HandleInteraction(GameObject* b, double dt)
{
}

void Shield::regenerateShield(float currHP, double dt)
{
    if (activeState == false && CurrHealth < MaxHealth)
    {
        CurrHealth += dt * 2;
    }
}

/******************************************************************************/
/*!
\brief	Destructor
/******************************************************************************/
Shield::~Shield()
{
}

/******************************************************************************/
/*!
\brief
Get Current Health to get the Current Health

\return
Current Shield Health
*/
/******************************************************************************/
float Shield::GetCurrHealth()
{
    return CurrHealth;
}

/******************************************************************************/
/*!
\brief0.

Get Max Health to get the Max Health

\return
Max Shield Health
*/
/******************************************************************************/
float Shield::GetMaxHealth()
{
    return MaxHealth;
}

/******************************************************************************/
/*!
\brief
Returns Projectile Shield Active State

\return
Projectile Shield Active State
*/
/******************************************************************************/
bool Shield::GetActiveState()
{
    return activeState;
}

/******************************************************************************/
/*!
\brief
Returns Projectile Shield Alive State

\return
Projectile Shield Alive State
*/
/******************************************************************************/
bool Shield::GetAliveState()
{
    return aliveState;
}

/******************************************************************************/
/*!
\brief
Setting the Current Shield Amount with a new amount;

\param curr
The current shield health to replace the old one
*/
/******************************************************************************/
void Shield::SetCurrHealth(float curr)
{
    CurrHealth = curr;
}

/******************************************************************************/
/*!
\brief
Setting the Max Shield Amount with a new amount;

\return
The max shield health to replace the old one
*/
/******************************************************************************/
void Shield::SetMaxHealth(float max)
{
    MaxHealth = max;
}
