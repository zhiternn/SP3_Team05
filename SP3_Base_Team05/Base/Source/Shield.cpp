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
Shield::Shield(WEAPON_TYPE weaponType) : 
CurrHealth(0),
MaxHealth(2000)
{

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
